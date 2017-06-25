/*
 * ForkPoseFilter.cpp
 *
 *  Created on: 21.08.2013
 *      Author: Messen
 */

#ifndef FORKPOSEFILTER_CPP_
#define FORKPOSEFILTER_CPP_

#include "ForkPoseFilter.h"

/**
 * Constructor
 */
dsa::ForkPoseFilter::ForkPoseFilter(Settings &settings) {
	settings_ = settings;
	cameraPosition_ = settings_.cameraPosition;
	rotationAxisPosition_ = settings_.rotationAxisPosition;
	forkTipDistance_ = settings_.forkTipDistance; // X-distance between axis and fork end
	heightMarkerOffset_ = settings_.heightMarkerOffset; // Y-distance between marker and fork
	initialCamRotation_ = settings_.initialCameraRotation; // rotation of the cam when the positions were measured

	// output:
	forkEndPosition = cv::Point2f(0,0); // The position of the fork end (Gabelspitze) after rotation.
	forkRotation = 0; // The rotation of the fork
	forkHeight = 0; // The height of the fork, if it would not be rotated.
	camRotation = cv::Mat::zeros(3,1,CV_32FC1); // The rotation of the camera

	// input:
	measuredHeightMarkerPosition = 0; // this is actually the position of the camera relative to the marker
	markers = 0; // The rotation marker

	// Values for exponential smoothing:
	smoothInitialized_ = false;
	smoothAlpha_ = 0.8f;

	Point2f rotMarkerPos = settings_.rotationMarkerPosition;
	// calculate distance between rotation marker and rotation axis - correct MJ
	markerAxisDist_ = sqrt(pow(rotationAxisPosition_.x-rotMarkerPos.x,2) + pow(rotationAxisPosition_.y-rotMarkerPos.y,2));
	// calculate distance between rotation axis and camera - correct MJ
	axisCamDist_ = sqrt(pow(rotationAxisPosition_.x-cameraPosition_.x,2) + pow(rotationAxisPosition_.y-cameraPosition_.y,2));

	// calculate the angle between marker and axis - correct MJ (not sure about the minus sign though)
	markerAxisAngle_ = -atan2f(rotMarkerPos.x-rotationAxisPosition_.x, rotMarkerPos.y-rotationAxisPosition_.y);

	// calculate the angle between the vector from cam to axis and the camera's looking direction (this is constant) - correct MJ (not sure about the sign though)
	angleCam_ = PI/2.f + acos((cameraPosition_.x-rotationAxisPosition_.x)/(axisCamDist_)) + initialCamRotation_;
}

/**
 * Destructor
 */
dsa::ForkPoseFilter::~ForkPoseFilter() {
}

void dsa::ForkPoseFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	text = std::string("(not set)");
}

/**
 * Calculates the height of the fork and its rotation
 */
void dsa::ForkPoseFilter::evaluate() {
	if(markers->size() > 0) { // rotation marker was detected
		// calculate marker center:
		aruco::Marker m = (*markers)[0];
		Point2f markerCoord = (m[0] + m[1] + m[2] + m[3]) * 0.25f;

		// calculate angle to the marker center (relative to camera)
		float fx = settings_.camParameters.CameraMatrix.at<float>(0,0);
		float cx = settings_.camParameters.CameraMatrix.at<float>(0,2);
		float measuredAngle = atan2f(markerCoord.x - cx, fx); // angle between vector from cam to marker and the direction of the cam


		// Axis, Marker, and camera build up a triangle
		// alpha = angle in Camera's corner
		// beta = angle in Marker's corner
		// gamma = angle in Axis' corner
		// x = angle between marker->axis and vertical line
		// y = angle between axis->marker and horizontal line
		// z = angle between axis->camera and horizontal line

		float alpha = angleCam_ - measuredAngle;

		// a / sin(alpha) = b / sin(beta) Sinussatz
		// =>  beta = asin( b/a * sin(alpha) )
		float beta = asin(axisCamDist_ /markerAxisDist_ * sin(alpha));

		float gamma = PI - alpha - beta;

		float y = PI/2.f - markerAxisAngle_;	// this seems to be wrong MJ (should be 90-(90-markerAxisAngle))
		float z = PI - gamma - y;

		float newForkRotation = alpha + measuredAngle - z - PI/2.f + 9.0/180.0*PI;

		/*
	float _minAngle = -6.73 * (PI/180.);
	float _maxAngle = 4.39 * (PI/180.);

	float _minMeasuredAngle = -6.56 * (PI/180.);
	float _maxMeasuredAngle = 5.44 * (PI/180.);

	newForkRotation = (newForkRotation - _minMeasuredAngle) * (_maxAngle-_minAngle)/(_maxMeasuredAngle-_minMeasuredAngle) + _minAngle;
		 */

		// smooth the rotation a bit (doesn't make much of a difference)
		if(!smoothInitialized_) {
			this->forkRotation = newForkRotation;
			smoothInitialized_ = true;
		} else {
			this->forkRotation = newForkRotation * smoothAlpha_ + forkRotation * (1.0f-smoothAlpha_);
		}

		camRotation = cv::Mat::zeros(3,1,CV_32FC1);
		camRotation.at<float>(1) = -(forkRotation - initialCamRotation_);

		// calculate the fork height like its not rotated
		this->forkHeight = cameraPosition_.y + -measuredHeightMarkerPosition->at<float>(2) - heightMarkerOffset_;

		// rotate the fork end point
		// like rotatePoint in helper.h but with floats
		// move center to rotationAxis
		float px = forkTipDistance_ - rotationAxisPosition_.x;
		float py = forkHeight - rotationAxisPosition_.y;
		// rotate around (0,0)
		float sina = sinf(-forkRotation);
		float cosa = cosf(-forkRotation);
		float temp = px;	// save x value temporarily for y calculation
		px = px*cosa - py*sina;
		py = py*cosa + temp*sina;
		// move center back to original position
		forkEndPosition = cv::Point2f(px + rotationAxisPosition_.x, py + rotationAxisPosition_.y);

		// create the info text
		text = std::string("Fork: ");
		text.append("H: ");
		text.append(doubleToString(forkHeight, 2));
		text.append(" R: ");
		text.append(doubleToString(forkRotation/PI*180.f, 2));
		text.append(" E: ");
		text.append(doubleToString(forkEndPosition.x, 2));
		text.append(", ");
		text.append(doubleToString(forkEndPosition.y, 2));
		text.append("); ");
		//text.append(doubleToString(x));
		//text.append("/");
		//text.append(doubleToString(z));
		//text.append("/");
		//text.append(doubleToString((int)(100*measuredAngle/PI*180.)/100.));
	}
}


/**
 * Fill internal data structure with markers
 */
/*
void ForkPoseFilter2::evaluate() {
	if(markers->size()==0) return; // rotation marker was not detected

	// calculate center:
	std::cout << "marker.size: " << markers->size() << std::endl;
	aruco::Marker m = (*markers)[0];
	Point2f markerCoord = (m[0] + m[1] + m[2] + m[3])*0.25;
	std::cout << "MarkerCoord: " << m[0] << std::endl;
	float fx = settings_.camParameters.CameraMatrix.at<float>(0,0);
	float cx = settings_.camParameters.CameraMatrix.at<float>(0,2);
	float measuredAngle = atan2f(markerCoord.x - cx, fx); // angle between vector from cam to marker and the direction of the cam

	measuredAngle = -PI/4.0f - PI/2.f;//PI/2.f;
	float initialCamRotation = -PI/4.0f;

	//float angleCam = PI*0.75; // angle between camera and rotation axis
	float angleCam = PI/2.f + acos((cameraPosition_.x-rotationAxisPosition_.x)/(axisCamDist_)) + initialCamRotation;
	std::cout << PI/2.f << " + " << "acos" << (cameraPosition_.x-rotationAxisPosition_.x)/(axisCamDist_) << std::endl;
	std::cout << "angleCam: " << (angleCam/PI*180.) << std::endl;
	float alpha = angleCam - measuredAngle; // angle between vector from cam to rotation axis and vector from cam to marker
	std::cout << "alpha: " << (alpha/PI*180.) << std::endl;
	// a / alpha = b / beta

	float beta = asin(axisCamDist_ / ( markerAxisDist_ / sin(alpha) ));
	std::cout << " - " << axisCamDist_ << " - " << markerAxisDist_ << std::endl;
	std::cout << "beta: " << (beta/PI*180.) << std::endl;

	float gamma = PI - alpha - beta;
	std::cout << "gamma: " << (gamma/PI*180.) << std::endl;

	float y = PI/2.f - markerAxisAngle_;
	std::cout << "y: " << (y/PI*180.) << std::endl;
	float z = PI - gamma - y;
	std::cout << "z: " << (z/PI*180.) << std::endl;
	float newForkRotation = alpha + measuredAngle - z - PI/2.f;


	float _minAngle = -6.73 * (PI/180.);
	float _maxAngle = 4.39 * (PI/180.);

	float _minMeasuredAngle = -11.96 * (PI/180.);
	float _maxMeasuredAngle = 6.036 * (PI/180.);

	//newForkRotation = (newForkRotation - _minMeasuredAngle) * (_maxAngle-_minAngle)/(_maxMeasuredAngle-_minMeasuredAngle) + _minAngle;


	float alphaFactor = 0.8f;
	if(!smoothInitialized_) {
		this->forkRotation = newForkRotation;
		smoothInitialized_ = true;
	} else {
		this->forkRotation = newForkRotation * alphaFactor + forkRotation * (1.0f-alphaFactor);
	}

	// TODO: Where to start measuring the fork height? its relative to the rotation...
	//float markerToFork = 56; // TODO: as parameter
	this->forkHeight = cameraPosition_.y + -measuredHeightMarkerPosition->at<float>(2) - heightMarkerOffset_;

	//TODO: what is a fitting offset? start measuring fork length at cam position?
	//float offset = cameraPosition_.x;

	cv::Point2f forkEnd = cv::Point2f(forkLength_, forkHeight);
	cv::Point2f p = rotatePoint(forkEnd , rotationAxisPosition_, forkRotation);

	forkEndPosition.at<float>(0) = 0;
	forkEndPosition.at<float>(1) = p.x;
	forkEndPosition.at<float>(2) = p.y;

	text = std::string("Fork: ");
	text.append("H: ");
	text.append(doubleToString((int)(100*forkHeight)/100.));
	text.append(" R: ");
	text.append(doubleToString((int)(100*forkRotation/PI*180.)/100.));
	text.append(" E: ");
	text.append(doubleToString((int)forkEndPosition.at<float>(1)));
	text.append(", ");
	text.append(doubleToString((int)forkEndPosition.at<float>(2)));
	text.append("); ");
	//text.append(doubleToString(x));
	//text.append("/");
	//text.append(doubleToString(z));
	//text.append("/");
	text.append(doubleToString((int)(100*measuredAngle/PI*180.)/100.));

}
 */

#endif /* FORKPOSEFILTER_CPP_ */

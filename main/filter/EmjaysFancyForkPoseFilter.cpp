/*
 * EmjaysFancyForkPoseFilter.cpp
 *
 *  Created on: 21.08.2013
 *      Author: Messen
 */

#include "EmjaysFancyForkPoseFilter.h"

/**
 * Constructor
 */
dsa::EmjaysFancyForkPoseFilter::EmjaysFancyForkPoseFilter(Settings &settings) {
	settings_ = settings;
	C = settings_.cameraPosition;
	R = settings_.rotationAxisPosition;
	forkTipDistance_ = settings_.forkTipDistance; // X-distance between axis and fork end
	heightMarkerOffset_ = settings_.heightMarkerOffset; // Y-distance between marker and fork
	initialCamRotation_ = settings_.initialCameraRotation; // rotation of the cam when the positions were measured
	rotationOffset_ = settings_.rotationOffset;
	heightOffset_ = settings_.heightOffset;

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

	M_R = settings_.rotationMarkerPosition;

	double e_1 = fabs(R.x - C.x);
	double e_2 = fabs(R.y - C.y);
	double a_1 = fabs(double(M_R.y - R.y));
	double a_2 = fabs(double(M_R.x - R.x));
	// calculate distance between rotation axis and camera - correct MJ
	double b_1 = sqrt(e_1*e_1 + e_2*e_2); // = e_3
	// calculate distance between rotation marker and rotation axis - correct MJ
	double b_2 = sqrt(a_1*a_1 + a_2*a_2);
	b_1byb_2 = b_1/b_2;

	alpha_2 = M_PI/2.0 - atan2(a_1, a_2);
	// calculate the angle between the vector from cam to axis and the camera's looking direction (this is constant) - correct MJ
	delta_2 = M_PI - atan2(e_1, e_2) - initialCamRotation_;

	x_min = settings.x_markerMin;	// measure with calibration (average)
	x_max = settings.x_markerMax;	// measure with calibration (average)
	y_min = settings.y_markerMin;	// measure with calibration (average)
	y_max = settings.y_markerMax;	// measure with calibration (average)
	imagePathLength = sqrt(pow((x_max-x_min),2) + pow((y_max-y_min),2));
	completeAngle = settings.fullTiltAngle;	// laut Datenblatt, nächstes Jahr genau messen
	zeroAngleOffset = settings.zeroTiltAngleOffset;
	haveValue = false;
}

/**
 * Destructor
 */
dsa::EmjaysFancyForkPoseFilter::~EmjaysFancyForkPoseFilter() {
}

void dsa::EmjaysFancyForkPoseFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	text = std::string("(not set)");
}

/**
 * Calculates the height of the fork and its rotation
 */
void dsa::EmjaysFancyForkPoseFilter::evaluate() {
	if(markers->size() > 0) { // rotation marker was detected
		haveValue = true;
		// calculate marker center:
		aruco::Marker m = (*markers)[0];
		double x_i = 0.25*(m[0].x + m[1].x + m[2].x + m[3].x);
		double y_i = 0.25*(m[0].y + m[1].y + m[2].y + m[3].y);

		// map path of the marker center in the image to the total tilt angle radians
		double newForkRotation = completeAngle * (1.0 - sqrt(pow((x_i-x_min),2) + pow((y_i-y_min),2)) / imagePathLength) + zeroAngleOffset;
		//std::cout << "markerCoords " << markerCoord_x << "," << cx << "," << fx << std::endl;
		//std::cout << "angles: " << delta_3 << "," << newForkRotation << std::endl;

		// smooth the rotation a bit (doesn't make much of a difference)
		if(!smoothInitialized_) {
			forkRotation = newForkRotation;
			smoothInitialized_ = true;
		} else {
			forkRotation = newForkRotation * smoothAlpha_ + forkRotation * (1.0-smoothAlpha_);
		}

		camRotation = cv::Mat::zeros(3,1,CV_32FC1);
		//camRotation.at<float>(1) = -(forkRotation - initialCamRotation_);
		camRotation.at<float>(1) = forkRotation;

		// calculate the fork height like its not rotated
		this->forkHeight = measuredHeightMarkerPosition->at<float>(2) + heightOffset_;

		// rotate the fork end point
		// like rotatePoint in helper.h but with floats
		// move center to rotationAxis
		double px = forkTipDistance_ - R.x;
		double py = forkHeight - R.y;
		// rotate around (0,0)
		float sina = sin(-forkRotation);
		float cosa = cos(-forkRotation);
		float temp = px;	// save x value temporarily for y calculation
		px = px*cosa - py*sina;
		py = py*cosa + temp*sina;
		// move center back to original position
		forkEndPosition = cv::Point2f(px + R.x, py + R.y);

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
	}
	else {
		haveValue = false;
	}
}

/**
 * Calculates the height of the fork and its rotation
 */
/*void dsa::EmjaysFancyForkPoseFilter::evaluate() {
	if(markers->size() > 0) { // rotation marker was detected
		// calculate marker center:
		aruco::Marker m = (*markers)[0];
		double markerCoord_x = 0.25*(m[0].x + m[1].x + m[2].x + m[3].x);
		double markerCoord_y = 0.25*(m[0].y + m[1].y + m[2].y + m[3].y);

		std::cerr << "markerCoord_x,y=" << markerCoord_x << "," << markerCoord_y << std::endl;

		// calculate angle to the marker center (relative to camera)
		double fx = settings_.camParameters.CameraMatrix.at<float>(0,0);
		double cx = settings_.camParameters.CameraMatrix.at<float>(0,2);

		double delta_3 = atan2(fabs(markerCoord_x - cx), fx); // angle between vector from cam to marker and the direction of the cam

		// compute rotation angle delta_1
		double delta_1 = -delta_3 + asin(b_1byb_2*sin(delta_2-delta_3)) + alpha_2;
		double scaleAngle = 0.76;
		double zeroForkAngle = 0.1600;	// calibration offset, no idea why this is necessary. make configurable
		double newForkRotation = scaleAngle * (-delta_1 + zeroForkAngle);

		//std::cout << "markerCoords " << markerCoord_x << "," << cx << "," << fx << std::endl;
		//std::cout << "angles: " << delta_3 << "," << newForkRotation << std::endl;

		// smooth the rotation a bit (doesn't make much of a difference)
		if(!smoothInitialized_) {
			forkRotation = newForkRotation;
			smoothInitialized_ = true;
		} else {
			forkRotation = newForkRotation * smoothAlpha_ + forkRotation * (1.0-smoothAlpha_);
		}

		camRotation = cv::Mat::zeros(3,1,CV_32FC1);
		//camRotation.at<float>(1) = -(forkRotation - initialCamRotation_);
		camRotation.at<float>(1) = forkRotation;

		// calculate the fork height like its not rotated
		this->forkHeight = C.y + -measuredHeightMarkerPosition->at<float>(2) - heightMarkerOffset_;

		// rotate the fork end point
		// like rotatePoint in helper.h but with floats
		// move center to rotationAxis
		double px = forkTipDistance_ - R.x;
		double py = forkHeight - R.y;
		// rotate around (0,0)
		float sina = sin(-forkRotation);
		float cosa = cos(-forkRotation);
		float temp = px;	// save x value temporarily for y calculation
		px = px*cosa - py*sina;
		py = py*cosa + temp*sina;
		// move center back to original position
		forkEndPosition = cv::Point2f(px + R.x, py + R.y);

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
	}
}*/

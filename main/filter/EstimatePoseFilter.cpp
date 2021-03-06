/*
 * EstimatePoseFilter.h
 *
 *	This filter extracts AR Markers from camera images using the aruco library.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "EstimatePoseFilter.h"

/**
 * Constructor
 */
dsa::EstimatePoseFilter::EstimatePoseFilter(Settings &settings, int transCoords, int rotCoords, int poseEstAlgoOverride, int regulationStrategyOverride) {
	haveValue = false;
	transCoords_ = transCoords;
	rotCoords_ = rotCoords;
	text = std::string("");

	settings_ = settings;

	// it depends on the use case which methods work best, so the settings can be overruled
	PoseEstimationAlgorithm poseEstAlgoEnum = (poseEstAlgoOverride > 0) ? (PoseEstimationAlgorithm)poseEstAlgoOverride : settings_.poseEstimationAlgorithm;
	RegulationStrategy regulationStrategyEnum = (regulationStrategyOverride > 0) ? (RegulationStrategy)regulationStrategyOverride : settings_.regulationStrategy;

	smoothPos_ = ExponentialSmoothing(settings_.exponentialSmoothingAlpha);
	smoothRot_ = ExponentialSmoothing(settings_.exponentialSmoothingAlpha, true);
	locmulti_ = LocalizorMulti(settings_.camParameters, poseEstAlgoEnum, regulationStrategyEnum, settings_.multiMarkerStrategy);

	camPosition = cv::Mat::zeros(3, 1, CV_32FC1);
	camRotation = cv::Mat::zeros(3, 1, CV_32FC1);

	//markers_ = std::vector<aruco::Marker>();
	//poses_ = std::vector<MarkerPose>();

	init();
}

/**
 * Destructor
 */
dsa::EstimatePoseFilter::~EstimatePoseFilter() {
}

void dsa::EstimatePoseFilter::init() {
	//markers = &markers_;
	//poses = &poses_;
}

void dsa::EstimatePoseFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	text = std::string("(no detection)");	
}

/**
 * Fill internal data structure with markers
 */
void dsa::EstimatePoseFilter::evaluate() {
	if(poses->size() > 0) {
      
		haveValue = true;
		Mat newPosition, newRotation;
		camPosition.copyTo(newPosition);
		camRotation.copyTo(newRotation);

		//estimate pose
		std::vector<MarkerPose> correctedPoses;
		for(size_t i = 0; i < poses->size(); i++) {
			MarkerPose p = (*poses)[i].copy();
			if(settings_.invertYAxis) {
				p.position.at<float>(1) *= -1;
			}
			correctedPoses.push_back(p);
		}

		if(settings_.invertYAxis && newPosition.cols == 1 && newPosition.rows == 3) {
			newPosition.at<float>(1) *= -1;
		}

		locmulti_.estimatePose(*markers, correctedPoses, newPosition, newRotation);
		// The rotation should be clockwise, therefore invert it:
		newRotation *= -1;

		if(settings_.invertYAxis) {
			newPosition.at<float>(1) *= -1;
		}

		/*newRotation.at<float>(0,0) = 0; // remove x rotation
	newRotation.at<float>(0,1) = 0; // remove y rotation
	newPosition.at<float>(0,2) = 0; // remove z position*/

		// remove translation coordinates which are not configured
		if((transCoords_ & 1) == 0)
			newPosition.at<float>(0,0) = 0; // remove x position
		if((transCoords_ & 2) == 0)
			newPosition.at<float>(0,1) = 0; // remove y position
		if((transCoords_ & 4) == 0)
			newPosition.at<float>(0,2) = 0; // remove z position

		// remove rotation coordinates which are not configured
		if((rotCoords_ & 1) == 0)
			newRotation.at<float>(0,0) = 0; // remove x rotation
		if((rotCoords_ & 2) == 0)
			newRotation.at<float>(0,1) = 0; // remove y rotation
		if((rotCoords_ & 4) == 0)
			newRotation.at<float>(0,2) = 0; // remove z rotation

		smoothPos_.add(newPosition);
		smoothRot_.add(newRotation);
		smoothPos_.get(camPosition);
		smoothRot_.get(camRotation);

		text = std::string("t=(");
		text.append(doubleToString((int)camPosition.at<float>(0, 0)));
		text.append(", ");
		text.append(doubleToString((int)camPosition.at<float>(0, 1)));
		text.append(", ");
		text.append(doubleToString((int)camPosition.at<float>(0, 2)));
		text.append("); ");
		text.append("r=(");
		text.append(doubleToString(int(180.0f*camRotation.at<float>(0, 0)/PI)));
		text.append(", ");
		text.append(doubleToString(int(180.0f*camRotation.at<float>(0, 1)/PI)));
		text.append(", ");
		text.append(doubleToString(int(180.0f*camRotation.at<float>(0, 2)/PI)));
		text.append(");");
	} else {
		haveValue = false;
	}


}

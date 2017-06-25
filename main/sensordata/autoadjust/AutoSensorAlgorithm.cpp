/*
 * AutoSensorAlgorithm.cpp
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#include "AutoSensorAlgorithm.h"

#ifdef WITH_RASPICAM
dsa::AutoSensorAlgorithm::AutoSensorAlgorithm(raspicam::RaspiCam_Cv* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame) 
#else
dsa::AutoSensorAlgorithm::AutoSensorAlgorithm(cv::VideoCapture* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame) 
#endif
{
	capture_ = videoCapture;
	minValue_ = minVal;
	maxValue_ = maxVal;
	property_ = property;
	everyNthFrame_ = everyNthFrame;
	run_ = 0;
	mask_ = cv::Mat(); // supply an empty mask in case no mask is specified
}

dsa::AutoSensorAlgorithm::~AutoSensorAlgorithm() {
}

double dsa::AutoSensorAlgorithm::getCurrentValue() {
	return(capture_->get(property_));
}

bool dsa::AutoSensorAlgorithm::needsUpdate() {
	return(run_ == 0);
}

bool dsa::AutoSensorAlgorithm::run(cv::Mat &image) {
	cv::Mat mask = cv::Mat();	// supply empty mask
	return(run(image, mask));
}

bool dsa::AutoSensorAlgorithm::run(cv::Mat &image, cv::Mat &mask) {
	bool updated = needsUpdate() ? update(image, mask) : false;
	run_++;
	run_ = run_%everyNthFrame_;
	return(updated);
}

/*
 * AutoExposureAlgorithm.cpp
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#include "AutoExposureAlgorithm.h"

#ifdef WITH_RASPICAM
dsa::AutoExposureAlgorithm::AutoExposureAlgorithm(raspicam::RaspiCam_Cv* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel) :
#else
dsa::AutoExposureAlgorithm::AutoExposureAlgorithm(cv::VideoCapture* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel) :
#endif
	AutoSensorAlgorithm(videoCapture, minVal, maxVal, property, everyNthFrame) 
{
	everyNthPixel_ = everyNthPixel;
	exposureTimeThreshold_ = 5.0;
}

dsa::AutoExposureAlgorithm::~AutoExposureAlgorithm() {
}

bool dsa::AutoExposureAlgorithm::update(cv::Mat &image, cv::Mat &mask) {
	double oldExposureTime, newExposureTime, factor;
	// calculate new exposureTime from old one
	oldExposureTime = getCurrentValue();
	if (oldExposureTime < 1.0)
		oldExposureTime = 1.0;

	factor = exp(1.5 * calcError(image, mask));
	newExposureTime = oldExposureTime * factor;

	bool overflow = false;
	if (newExposureTime > maxValue_) {
		newExposureTime = maxValue_;
		overflow = true;
	} else if (newExposureTime < minValue_) {
		newExposureTime = minValue_;
		overflow = true;
	}

	double diff = fabs((newExposureTime - oldExposureTime) / oldExposureTime) * 100.0;
	bool changeValue = diff >= exposureTimeThreshold_ || (overflow && newExposureTime != oldExposureTime);
	if(changeValue) {
		capture_->set(property_, newExposureTime);
	}
	return(changeValue);
}

double dsa::AutoExposureAlgorithm::getExposureTimeThreshold() {
	return(exposureTimeThreshold_);
}

void dsa::AutoExposureAlgorithm::setExposureTimeThreshold(double value) {
	exposureTimeThreshold_ = value;
}

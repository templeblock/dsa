/*
 * MeanAutoExposureAlgorithm.cpp
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#include "MeanAutoExposureAlgorithm.h"

#ifdef WITH_RASPICAM
dsa::MeanAutoExposureAlgorithm::MeanAutoExposureAlgorithm(raspicam::RaspiCam_Cv* const videoCapture, 
#else
dsa::MeanAutoExposureAlgorithm::MeanAutoExposureAlgorithm(cv::VideoCapture* const videoCapture, 
#endif
const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel) : AutoExposureAlgorithm(videoCapture, minVal, maxVal, property, everyNthFrame, everyNthPixel) {
	exposureTimeThreshold_ = 10.0;
}

dsa::MeanAutoExposureAlgorithm::~MeanAutoExposureAlgorithm() {
}

double dsa::MeanAutoExposureAlgorithm::calcError(cv::Mat &image, cv::Mat &mask) {
	const double avgPixel = 128.0;
	int nchannels = image.channels();
	cv::Scalar avg = cv::mean(image, mask);
	//cv::Scalar avg = cv::mean(image);

	double error = 0.0;
	for(size_t i = 0; i < nchannels; i++)
		error += avg.val[i];
	error = 1.0 - error/(nchannels*avgPixel);

	return(error);
}

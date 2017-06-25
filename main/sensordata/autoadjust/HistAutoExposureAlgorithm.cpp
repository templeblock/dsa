/*
 * HistAutoExposureAlgorithm.cpp
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#include "HistAutoExposureAlgorithm.h"

#ifdef WITH_RASPICAM
dsa::HistAutoExposureAlgorithm::HistAutoExposureAlgorithm(raspicam::RaspiCam_Cv* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel) : 
#else
dsa::HistAutoExposureAlgorithm::HistAutoExposureAlgorithm(cv::VideoCapture* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel) : 
#endif
	AutoExposureAlgorithm(videoCapture, minVal, maxVal, property, everyNthFrame, everyNthPixel) {
}

dsa::HistAutoExposureAlgorithm::~HistAutoExposureAlgorithm() {
}

double dsa::HistAutoExposureAlgorithm::calcError(cv::Mat &image, cv::Mat &mask) {
	const int threshMin = 2;
	const double threshRatio = 1.0/256.0; // 1 bucket in histogram (actually 2 buckets thought to be filled up to half times average each)

	size_t nPixels = image.rows * image.cols * image.channels();

	int nVeryDark = 0;
	int nVeryLight = 0;
	int thresh = 0;
	int threshPixels = 0;
	int histogram[256] = {0};

	// handle masked and unmasked case separately => much faster
	if(mask.empty()) {
		for(size_t i = 0; i < nPixels; i+=everyNthPixel_)
			histogram[image.data[i]]++;
		nPixels /= everyNthPixel_;
	} else {
		int maskedPixels = 0;
		for(size_t i = 0; i < nPixels; i+=everyNthPixel_) {
			int toAdd = mask.data[i];	// can be 1 or 0
			histogram[image.data[i]] += toAdd;
			maskedPixels += toAdd;
		}
		nPixels = maskedPixels;
	}

	int threshMaxPixels = (int)(nPixels * threshRatio);
	while(thresh < threshMin || threshPixels < threshMaxPixels) {
		nVeryDark += histogram[thresh];
		nVeryLight += histogram[255-thresh];
		threshPixels = std::max(nVeryDark, nVeryLight);
		thresh++;
	}

	// large very dark areas are likely due to underexposure, whilst large very light areas indicate overexposure
	double error;
	if(nVeryLight < nVeryDark) {
		// underexposure (too dark)
		size_t i = 255-thresh;
		UINT32 sum = nVeryLight;
		while((sum = sum+histogram[i]) < nVeryDark)
			i--;
		error = (255-thresh-i)/256.0;
	} else { // overexposure
		size_t i = thresh;
		UINT32 sum = nVeryDark;
		while((sum = sum+histogram[i]) < nVeryLight)
			i++;
		error = (i-thresh)/-256.0;
	}

	return(error);
}

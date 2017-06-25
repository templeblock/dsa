/*
 * HybridAutoExposureAlgorithm.cpp
 *
 * The HistAutoExposureAlgorithm preserves the information in the image well by choosing an exposure time to prevent whichever of overexposure or underexposure is worse at the moment.
 * Since overexposure is usually worse, it will favor low exposure times to prevent overexposure, which leads to preservation of information at the cost of a bad signal to noise ratio.
 * This approach is a compromise between preserving information and reducing noise. It features both the HistAutoExposureAlgorithm and MeanAutoExposureAlgorithm and tries fits them
 * together by calculating their errors separately and build the weighted sum over both errors. The weight can either be specified statically via the "weight" parameter, or automatically
 * be determined (see below).
 *
 * A better method would probably be, to specify a desireable noise level (or interval), however the result would be very camera specific and difficult to evaluate. Also specifying the
 * proper noise level could be a pain for systems integrators, therefore such a method is considered out of the focus of our research project.
 * At the moment this algorithm suffer from the same instability as the meanvalue algorithm does (it cannot always find a stable exposure value but will jump between different values because
 * neither is within exposureTimeThreshold_).
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#include "HybridAutoExposureAlgorithm.h"

dsa::HybridAutoExposureAlgorithm::HybridAutoExposureAlgorithm(cv::VideoCapture* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel, const double weight) : AutoExposureAlgorithm(videoCapture, minVal, maxVal, property, everyNthFrame, everyNthPixel) {
	weight_ = weight;

	exposureTimeThreshold_ = 10.0;
}

dsa::HybridAutoExposureAlgorithm::~HybridAutoExposureAlgorithm() {
}

/**
 * Calculate the error.
 * To speed the algorithm up, we compute everything here instead of using HistAutoExposureAlgorithm::calcError().
 * As a result, it should be almost as fast as HistAutoExposure, which is very close to the standard mean method.
 * You can further speed up the process by simply feeding it a smaller (resized) version of the image (if you have to calculate that one anyway).
 *
 * @param image
 * @param mask
 * @return
 */
double dsa::HybridAutoExposureAlgorithm::calcError(cv::Mat &image, cv::Mat &mask) {
	// we cannot inherit from both, because they come from the same base-class, so unfortunately we cannot use calcError() from MeanAutoExposure
	const int threshMin = 2;
	const double avgPixel = 128.0;
	const double threshRatio = 1.0/256.0; // 1 bucket in histogram (actually 2 buckets thought to be filled up to half times average each)

	UINT32 nPixels = image.rows * image.cols;
	int nVeryDark = 0, nVeryLight = 0;
	int thresh = 0, threshPixels = 0;
	int histogram[256] = {0};
	UINT32 sum = 0;	// UINT32 could be too few for 16MPixel, int even for 8MPixel, but we don't care as for that large amount of pixel we will only choose every n't pixel with big n

	double error1, error2;
	// handle masked and unmasked case separately => much faster as we save all the conditional branches
	if(mask.empty()) {
		for(size_t i = 0; i < nPixels; i+=everyNthPixel_) {
			sum += image.data[i];
			histogram[image.data[i]]++;
		}
	} else {
		int maskedPixels = 0;
		for(size_t i = 0; i < nPixels; i+=everyNthPixel_) {
			int toAdd = mask.data[i];	// can be 1 or 0
			int pixelValue = image.data[i];
			histogram[pixelValue] += toAdd;
			sum += (toAdd * pixelValue);
			maskedPixels += toAdd;
		}
		nPixels = maskedPixels;
	}

	// calc error one
	double mean = (double)sum/nPixels;
	error1 = 1.0 - mean/avgPixel;

	// calc error2
	int threshMaxPixels = (int)(nPixels * threshRatio);
	while(thresh < threshMin || threshPixels < threshMaxPixels) {
		nVeryDark += histogram[thresh];
		nVeryLight += histogram[255-thresh];
		threshPixels = std::max(nVeryDark, nVeryLight);
		thresh++;
	}

	// large very dark areas are likely due to underexposure, whilst large very light areas indicate overexposure
	if(nVeryLight < nVeryDark) { // underexposure (too dark)
		size_t i = 255-thresh;
		UINT32 sum = nVeryLight;
		while((sum += histogram[i]) < nVeryDark)
			i--;
		error2 = (255-thresh-i)/256.0;
	} else { // overexposure
		size_t i = thresh;
		UINT32 sum = nVeryDark;
		while((sum += histogram[i]) < nVeryLight)
			i++;
		error2 = (i-thresh)/-256.0;
	}

	if(weight_ < 0.0) { // automagically guess a good value
		if(mean >= avgPixel || error1*error2 > 0.0) // either already white enough or both would be pulling into the same direction anyway, so we are fine with HistAE alone
			return(error2);
		else { // too dark because of HistAE => use MeanAE to correct
			double meanWeight = fabs(error1); // weight itself with the severity of the error
			return((error1*meanWeight + error2)/(1.0+meanWeight));
		}
	} else { // or apply specified weight
		error1 *= (1.0 - weight_);	// mean method
		error2 *= weight_;			// histogram method
		return(error1+error2);
	}
}

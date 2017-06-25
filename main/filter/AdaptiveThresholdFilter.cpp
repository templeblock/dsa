/*
 * AdaptiveThresholdFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#adaptivethreshold
 *
 *	for details on the image filter.
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#include "AdaptiveThresholdFilter.h"

/**
 * Constructor
 *
 * @param maxValue: Non-zero value assigned to the pixels for which the condition is satisfied
 * @param adaptiveMethod: Adaptive thresholding algorithm to use, ADAPTIVE_THRESH_MEAN_C or ADAPTIVE_THRESH_GAUSSIAN_C
 * @param thresholdType: Thresholding type that must be either THRESH_BINARY or THRESH_BINARY_INV
 * @param blockSize: Size of a pixel neighborhood that is used to calculate a threshold value for the pixel: 3, 5, 7, and so on
 * @param C: Constant subtracted from the mean or weighted mean (see the details below). Normally, it is positive but may be zero or negative as well.
 */
dsa::AdaptiveThresholdFilter::AdaptiveThresholdFilter(double maxValue, int adaptiveMethod, int thresholdType, int blockSize, double C) {
	maxValue_ = maxValue;
	adaptiveMethod_ = adaptiveMethod;
	thresholdType_ = thresholdType;
	blockSize_ = blockSize;
	C_ = C;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::AdaptiveThresholdFilter::~AdaptiveThresholdFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::AdaptiveThresholdFilter::init() {
	maxValue = &maxValue_;
	adaptiveMethod = &adaptiveMethod_;
	thresholdType = &thresholdType_;
	blockSize = &blockSize_;
	C = &C_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::AdaptiveThresholdFilter::apply() {
	if(isReady()) {
		cv::adaptiveThreshold(*src, dst, *maxValue, *adaptiveMethod, *thresholdType, *blockSize, *C);
	}
}

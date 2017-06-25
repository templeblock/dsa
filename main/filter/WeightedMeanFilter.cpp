/*
 * WeightedMeanFilter.cpp
 *
 * 	See
 *
 *	2D Filter that assigns the mean value of the kernel to the pixel in the middle of the kernel
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "WeightedMeanFilter.h"

/**
 * Constructor
 *
 * @param ksize: Aperture linear size. It should be odd and must be greater than 1.
 * @param weight: Weight of the center point in the weighted mean as fraction of 1 (which is the sum of all weights)
 */
dsa::WeightedMeanFilter::WeightedMeanFilter(int ksize, float centerWeight) {
	kernel_ = cv::Mat::ones(ksize, ksize, CV_32F)*((1.0f-centerWeight)/(ksize*ksize-1));
	kernel_.at<float>(ksize >> 1, ksize >> 1) = centerWeight;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::WeightedMeanFilter::~WeightedMeanFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::WeightedMeanFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::WeightedMeanFilter::apply() {
	if(isReady()) {
		cv::filter2D(*src, dst, -1, kernel_);
	}
}

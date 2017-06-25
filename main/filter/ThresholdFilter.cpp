/*
 * ThresholdFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#threshold
 *
 *	for details on the image filter.
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#include "ThresholdFilter.h"

/**
 * Constructor
 *
 * @param thresh: Threshold value.
 * @param maxVal: Maximum value to use with the THRESH_BINARY and THRESH_BINARY_INV thresholding types.
 * @param thresholdType: Thresholding type.
 */
dsa::ThresholdFilter::ThresholdFilter(double thresh, double maxVal, int thresholdType) {
	thresh_ = thresh;
	maxVal_ =  maxVal;
	thresholdType_ = thresholdType;

	init ();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ThresholdFilter::~ThresholdFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::ThresholdFilter::init() {
	thresh = &thresh_;
	maxVal = &maxVal_;
	thresholdType = &thresholdType_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::ThresholdFilter::apply() {
	if(isReady()) {
		threshold(*src, dst, *thresh, *maxVal, *thresholdType);
	}
}

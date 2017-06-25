/*
 * MorphologyExFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#morphologyex
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "MorphologyExFilter.h"

/**
 * Constructor
 *
 * @param op: Type of a morphological operation.
 * @param element: Structuring element.
 * @param anchor: Pixel extrapolation method.
 * @param iterations: Number of times erosion and dilation are applied.
 * @param borderType: Pixel extrapolation method.
 * @param borderValue: Border value in case of a constant border.
 */
dsa::MorphologyExFilter::MorphologyExFilter(int op, cv::Mat& element, cv::Point anchor, int iterations, int borderType, const cv::Scalar& borderValue) {
	op_ = op;
	element_ = element;
	anchor_ = anchor;
	iterations_ = iterations;
	borderType_ = borderType;
	borderValue_ = borderValue;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::MorphologyExFilter::~MorphologyExFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::MorphologyExFilter::init() {
	op = &op_;
	element = &element_;
	anchor = &anchor_;
	iterations = &iterations_;
	borderType = &borderType_;
	borderValue = &borderValue_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::MorphologyExFilter::apply() {
	if(isReady()) {
		cv::morphologyEx(*src, dst, *op, *element, *anchor, *iterations, *borderType, *borderValue);
	}
}

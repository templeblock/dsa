/*
 * ErodeFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "ErodeFilter.h"

/**
 * Constructor
 *
 * @param element: Structuring element used for erosion. If element=Mat() , a 3 x 3 rectangular structuring element is used.
 * @param anchor: Position of the anchor within the element. The default value (-1, -1) means that the anchor is at the element center.
 * @param iterations: Number of times erosion is applied.
 * @param borderType: Pixel extrapolation method.
 * @param borderValue: Border value in case of a constant border.
 */
dsa::ErodeFilter::ErodeFilter(cv::Mat& element, cv::Point anchor, int iterations, int borderType, const cv::Scalar& borderValue) {
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
dsa::ErodeFilter::~ErodeFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::ErodeFilter::init() {
	element = &element_;
	anchor = &anchor_;
	iterations = &iterations_;
	borderType = &borderType_;
	borderValue = &borderValue_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::ErodeFilter::apply() {
	if(isReady()) {
		cv::erode(*src, dst, *element, *anchor, *iterations, *borderType, *borderValue);
	}
}

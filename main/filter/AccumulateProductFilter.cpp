/*
 * AccumulateProductFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/motion_analysis_and_object_tracking.html#accumulateproduct
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "AccumulateProductFilter.h"

/**
 * Constructor
 *
 * @param src2: Second input image of the same type and the same size as src.
 * @param mask: Optional operation mask.
 */
dsa::AccumulateProductFilter::AccumulateProductFilter(const cv::Mat& src2, const cv::Mat& mask) {
	src2_ = src2;
	mask_ = mask;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::AccumulateProductFilter::~AccumulateProductFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::AccumulateProductFilter::init() {
	src2 = &src2_;
	mask = &mask_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::AccumulateProductFilter::apply() {
	if(isReady()) {
		cv::accumulateProduct(*src, *src2, dst, *mask);
	}
}

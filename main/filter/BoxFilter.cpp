/*
 * BoxFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#boxfilter
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "BoxFilter.h"

/**
 * Constructor
 *
 * @param ddepth
 * @param ksize: Smoothing kernel size.
 * @param anchor: Anchor point. The default value Point(-1,-1) means that the anchor is at the kernel center.
 * @param normalize: Flag specifying whether the kernel is normalized by its area or not.
 * @param borderType: Border mode used to extrapolate pixels outside of the image.
 */
dsa::BoxFilter::BoxFilter(int ddepth, cv::Size ksize, cv::Point anchor, bool normalize, int borderType) {
	ddepth_ = ddepth;
	ksize_ = ksize;
	anchor_ = anchor;
	normalize_ = normalize;
	borderType_ = borderType;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BoxFilter::~BoxFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BoxFilter::init() {
	ksize = &ksize_;
	anchor = &anchor_;
	normalize = &normalize_;
	borderType = &borderType_;
	ddepth = &ddepth_;
}

/**
 * When filter is ready, run the OpenCV function.
 */

void dsa::BoxFilter::apply() {
	if(isReady()) {
		cv::boxFilter(*src, dst, *ddepth, *ksize, *anchor, *normalize, *borderType);
	}
}

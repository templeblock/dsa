/*
 * BlurFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#blur
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "BlurFilter.h"

/**
 * Constructor
 *
 * @param ksize: Smoothing kernel size.
 * @param anchor: Anchor point. The default value Point(-1,-1) means that the anchor is at the kernel center.
 * @param borderType: Border mode used to extrapolate pixels outside of the image.
 */
dsa::BlurFilter::BlurFilter(cv::Size ksize, cv::Point anchor, int borderType) {
	ksize_ = ksize;
	anchor_ = anchor;
	borderType_ = borderType;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BlurFilter::~BlurFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BlurFilter::init() {
	ksize = &ksize_;
	anchor = &anchor_;
	borderType = &borderType_;
}

/**
 * When filter is ready, run the OpenCV function.
 */

void dsa::BlurFilter::apply() {
	if(isReady()) {
		cv::blur(*src, dst, *ksize, *anchor, *borderType);
	}
}

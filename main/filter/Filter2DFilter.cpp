/*
 * Filter2DFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#filter2d
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "Filter2DFilter.h"

/**
 * Constructor
 *
 * @param ddepth: Desired depth of the destination image.
 * @param kernel: Convolution kernel (or rather a correlation kernel), a single-channel floating point matrix.
 * @param anchor: Anchor of the kernel that indicates the relative position of a filtered point within the kernel. The anchor should lie within the kernel. The special default value (-1,-1) means that the anchor is at the kernel center.
 * @param delta: Optional value added to the filtered pixels before storing them in dst.
 * @param borderType: Pixel extrapolation method.
 */
dsa::Filter2DFilter::Filter2DFilter(int ddepth, cv::Mat& kernel, cv::Point anchor, double delta, int borderType) {
	ddepth_ = ddepth;
	kernel_ = kernel;
	anchor_ = anchor;
	delta_ = delta;
	borderType_ = borderType;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::Filter2DFilter::~Filter2DFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::Filter2DFilter::init() {
	ddepth = &ddepth_;
	kernel = &kernel_;
	anchor = &anchor_;
	delta = &delta_;
	borderType = &borderType_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::Filter2DFilter::apply() {
	if(isReady()) {
		cv::filter2D(*src, dst, *ddepth, *kernel, *anchor, *delta, *borderType);
	}
}

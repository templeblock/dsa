/*
 * PyrDownFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#pyrdown
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "PyrDownFilter.h"

/**
 * Constructor
 *
 * @param dstsize: Size of the destination image. By default, it is computed as Size((src.cols+1)/2, (src.rows+1)/2).
 */
dsa::PyrDownFilter::PyrDownFilter(const cv::Size& dstsize) {
	dstsize_ = dstsize;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::PyrDownFilter::~PyrDownFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::PyrDownFilter::init() {
	dstsize = &dstsize_;;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::PyrDownFilter::apply() {
	if(isReady()) {
		cv::pyrDown(*src, dst, *dstsize);
	}
}

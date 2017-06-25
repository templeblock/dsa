/*
 * ResizeFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#resize
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "ResizeFilter.h"

/**
 * Constructor
 *
 * Initialize the filter to resize based on a fixed dst size. Standard interpolation method is INTER_LINEAR, as it is the fastest with reasonable quality.
 * If you want a different value, just set it afterwards.
 *
 * @param dsize the size of the dst image
 */
dsa::ResizeFilter::ResizeFilter(cv::Size dsize, int interpolation) {
	dsize_ = dsize;
	fx_ = 0;
	fy_ = 0;
	interpolation_ = interpolation;

	init();
}

/**
 * Constructor
 *
 * Initialize the filter to resize based on a factor to symmetrically increase the src height and width by. Standard interpolation method is
 * INTER_LINEAR, as it is the fastest with reasonable quality. If you want a different value, just set it afterwards.

 * @param fxy factor to increase the src width and height by
 */
dsa::ResizeFilter::ResizeFilter(double fxy, int interpolation) {
	dsize_ = cv::Size();
	fx_ = fxy;
	fy_ = fxy;
	interpolation_ = interpolation;

	init();
}

/**
 * Constructor
 *
 * Initialize the filter to resize based on a factors to increase the src height and width by. Standard interpolation method is
 * INTER_LINEAR, as it is the fastest. If you want a different value, just set it afterwards.

 * @param fx factor to increase the src width by
 * @param fy factor to increase the src height by
 */
dsa::ResizeFilter::ResizeFilter(double fx, double fy, int interpolation) {
	dsize_ = cv::Size();
	fx_ = fx;
	fy_ = fy;
	interpolation_ = interpolation;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ResizeFilter::~ResizeFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::ResizeFilter::init() {
	dsize = &dsize_;
	fx = &fx_;
	fy = &fy_;
	interpolation = &interpolation_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::ResizeFilter::apply() {
	if(isReady()) {
		cv::resize(*src, dst, *dsize, *fx, *fy, *interpolation);
	}
}

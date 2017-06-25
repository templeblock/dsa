/*
 * ScharrFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "ScharrFilter.h"

/**
 * Constructor
 *
 * @param ddepth: Destination image depth.
 * @param xorder: Order of the derivative x.
 * @param yorder: Order of the derivative y.
 * @param scale: Optional scale factor for the computed derivative values. By default, no scaling is applied.
 * @param delta: Optional delta value that is added to the results prior to storing them in dst.
 * @param borderType: Pixel extrapolation method.
 */
dsa::ScharrFilter::ScharrFilter(int ddepth, int xorder, int yorder, double scale, double delta, int borderType) {
	ddepth_ = ddepth;
	xorder_ = xorder;
	yorder_ = yorder ;
	scale_ = scale;
	delta_ = delta;
	borderType_ = borderType;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ScharrFilter::~ScharrFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::ScharrFilter::init() {
	ddepth = &ddepth_;
	xorder = &xorder_;
	yorder = &yorder_;
	scale = &scale_;
	delta = &delta_;
	borderType = &borderType_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::ScharrFilter::apply() {
	if(isReady()) {
		Scharr(*src, dst, *ddepth, *xorder, *yorder, *scale, *delta, *borderType);
	}
}

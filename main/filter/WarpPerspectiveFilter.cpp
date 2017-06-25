/*
 * WarpPerspectiveFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#warpperspective
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "WarpPerspectiveFilter.h"

/**
 * Constructor
 *
 * @param M: 3x3 transformation matrix.
 * @param dsize: Size of the destination image.
 * @param flags: Combination of interpolation methods and the optional flag WARP_INVERSE_MAP that means that M is the inverse transformation.
 * @param borderMode: Pixel extrapolation method. When borderMode=BORDER_TRANSPARENT, it means that the pixels in the destination image that corresponds to the “outliers” in the source image are not modified by the function.
 * @param borderValue: Value used in case of a constant border. By default, it is 0.
 */
dsa::WarpPerspectiveFilter::WarpPerspectiveFilter(cv::Mat& M, cv::Size dsize, int flags, int borderMode, const cv::Scalar& borderValue) {
	M_ = M;
	dsize_ = dsize;
	flags_ = flags;
	borderMode_ = borderMode;
	borderValue_ = borderValue;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::WarpPerspectiveFilter::~WarpPerspectiveFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::WarpPerspectiveFilter::init() {
	M = &M_;
	dsize = &dsize_;
	flags = &flags_;
	borderMode = &borderMode_;
	borderValue = &borderValue_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::WarpPerspectiveFilter::apply() {
	if(isReady()) {
		cv::warpPerspective(*src, dst, *M, *dsize, *flags, *borderMode, *borderValue);
	}
}

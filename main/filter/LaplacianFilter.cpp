/*
 * LaplacianFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#laplacian
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "LaplacianFilter.h"

/**
 * Constructor
 *
 * @param ddepth: Desired depth of the destination image.
 * @param ksize: Aperture size used to compute the second-derivative filters. The size must be positive and odd.
 * @param scale: Optional scale factor for the computed Laplacian values. By default, no scaling is applied.
 * @param delta: Optional delta value that is added to the results prior to storing them in dst .
 * @param borderType: Pixel extrapolation method.
 */
dsa::LaplacianFilter::LaplacianFilter(int ddepth, int ksize, double scale, double delta, int borderType) {
	ddepth_ = ddepth;
	ksize_ = ksize;
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
dsa::LaplacianFilter::~LaplacianFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::LaplacianFilter::init() {
	ddepth = &ddepth_;
	ksize = &ksize_;
	scale = &scale_;
	delta = &delta_;
	borderType = &borderType_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::LaplacianFilter::apply() {
	if(isReady()) {
		cv::Laplacian(*src, dst, *ddepth, *ksize, *scale, *delta, *borderType);
	}
}

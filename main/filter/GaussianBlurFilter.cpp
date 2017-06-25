/*
 * GaussianBlurFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#gaussianblur
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "GaussianBlurFilter.h"

/**
 * Constructor
 *
 * @param ksize: Gaussian kernel size. ksize.width and ksize.height can differ but they both must be positive and odd. Or, they can be zero’s and then they are computed from sigma*.
 * @param sigmaX: Gaussian kernel standard deviation in X direction.
 * @param sigmaY: Gaussian kernel standard deviation in Y direction. If sigmaY is zero, it is set to be equal to sigmaX. If both sigmas are zeros, they are computed from ksize.width and ksize.height , respectively.
 * @param borderType: Pixel extrapolation method.
 */
dsa::GaussianBlurFilter::GaussianBlurFilter(cv::Size ksize, double sigmaX, double sigmaY, int borderType) {
	ksize_ = ksize;
	sigmaX_ = sigmaX;
	sigmaY_ = sigmaY;
	borderType_ = borderType;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::GaussianBlurFilter::~GaussianBlurFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::GaussianBlurFilter::init() {
	 ksize = &ksize_;
	 sigmaX = &sigmaX_;
	 sigmaY = &sigmaY_;
	 borderType = &borderType_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::GaussianBlurFilter::apply() {
	if(isReady()) {
		cv::GaussianBlur(*src, dst, *ksize, *sigmaX, *sigmaY, *borderType);
	}
}

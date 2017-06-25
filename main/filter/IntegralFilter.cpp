/*
 * IntegralFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#integral
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "IntegralFilter.h"

/**
 * Constructor
 *
 * @param sum: Integral image as (W+1) x (H+1) , 32-bit integer or floating-point (32f or 64f).
 * @param sdepth: Desired depth of the integral and the tilted integral images, CV_32S, CV_32F, or CV_64F.
 */
dsa::IntegralFilter::IntegralFilter(cv::Mat& sum, int sdepth) {
	sum_ = sum;
	sdepth_ = sdepth;

	mode_ = S;
	init();
}

/**
 * Constructor
 *
 * @param sum: Integral image as (W+1) x (H+1) , 32-bit integer or floating-point (32f or 64f).
 * @param sqsum: Integral image for squared pixel values. It is (W+1) x (H+1), double-precision floating-point (64f) array.
 * @param sdepth: Desired depth of the integral and the tilted integral images, CV_32S, CV_32F, or CV_64F.
 */
dsa::IntegralFilter::IntegralFilter(cv::Mat& sum, cv::Mat& sqsum, int sdepth) {
	sum_ = sum;
	sqsum_ = sqsum;
	sdepth_ = sdepth;

	mode_ = SS;
	init();
}

/**
 * Constructor
 *
 * @param sum: Integral image as (W+1) x (H+1) , 32-bit integer or floating-point (32f or 64f).
 * @param sqsum: Integral image for squared pixel values. It is (W+1) x (H+1), double-precision floating-point (64f) array.
 * @param tilted: Integral for the image rotated by 45 degrees. It is (W+1) x (H+1) array with the same data type as sum.
 * @param sdepth: Desired depth of the integral and the tilted integral images, CV_32S, CV_32F, or CV_64F.
 */
dsa::IntegralFilter::IntegralFilter(cv::Mat& sum, cv::Mat& sqsum, cv::Mat& tilted, int sdepth) {
	sum_ = sum;
	sqsum_ = sqsum;
	tilted_ = tilted;
	sdepth_ = sdepth;

	mode_ = SST;
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::IntegralFilter::~IntegralFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::IntegralFilter::init() {
	sum = &sum_;
	sqsum = &sqsum_;
	tilted = &tilted_;
	sdepth = &sdepth_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::IntegralFilter::evaluate() {
	switch(mode_) {
	case S:
		integral(*src, *sum, *sdepth);
		break;
	case SS:
		integral(*src, *sum, *sqsum, *sdepth);
		break;
	case SST:
		integral(*src, *sum, *sqsum, *tilted, *sdepth);
		break;
	}
}

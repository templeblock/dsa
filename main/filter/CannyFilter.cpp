/*
 * CannyFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "CannyFilter.h"

/**
 * Constructor
 *
 * @param threshold1: First threshold for the hysteresis procedure.
 * @param threshold2: Second threshold for the hysteresis procedure.
 * @param apertureSize: Aperture size for the Sobel() operator.
 * @param L2gradient: Flag indicating whether a more accurate L_2 norm should be used to compute the image gradient magnitude (L2gradient=true), or a faster default L_1 norm is enough (L2gradient=false).
 */
dsa::CannyFilter::CannyFilter(double threshold1, double threshold2, int apertureSize, bool L2gradient) {
	threshold1_ = threshold1;
	threshold2_ = threshold2;
	apertureSize_ = apertureSize;
	L2gradient_ = L2gradient;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::CannyFilter::~CannyFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::CannyFilter::init() {
	threshold1 = &threshold1_;
	threshold2 = &threshold2_;
	apertureSize = &apertureSize_;
	L2gradient = &L2gradient_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::CannyFilter::apply() {
	if(isReady()) {
		Canny(*src, dst, *threshold1, *threshold2, *apertureSize, *L2gradient);
	}
}

/*
 * BilateralFilterFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#bilateralfilter
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "BilateralFilter.h"

/**
 * Constructor
 *
 * @param d: Diameter of each pixel neighborhood that is used during filtering. If it is non-positive, it is computed from sigmaSpace .
 * @param sigmaColor: Filter sigma in the color space. A larger value of the parameter means that farther colors within the pixel neighborhood (see sigmaSpace ) will be mixed together, resulting in larger areas of semi-equal color.
 * @param sigmaSpace: Filter sigma in the coordinate space. A larger value of the parameter means that farther pixels will influence each other as long as their colors are close enough (see sigmaColor ). When d>0 , it specifies the neighborhood size regardless of sigmaSpace . Otherwise, d is proportional to sigmaSpace
 * @param borderType
 */
dsa::BilateralFilter::BilateralFilter(int d, double sigmaColor, double sigmaSpace, int borderType) {
	d_ = d;
	sigmaColor_= sigmaColor;
	sigmaSpace_= sigmaSpace;
	borderType_ = borderType;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BilateralFilter::~BilateralFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BilateralFilter::init() {
	d = &d_;
	sigmaColor = &sigmaColor_;
	sigmaSpace = &sigmaSpace_;
	borderType = &borderType_;
}

/**
 * When filter is ready, run the OpenCV function.
 */

void dsa::BilateralFilter::apply() {
	if(isReady()) {
		cv::bilateralFilter(*src, dst, *d, *sigmaColor, *sigmaSpace, *borderType);
	}
}

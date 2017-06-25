/*
 * CopyMakeBorderFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#copymakeborder
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "CopyMakeBorderFilter.h"

/**
 * Constructor
 *
 * @param top: Parameter specifying how many pixels in each direction from the source image rectangle to extrapolate. For example, top=1, bottom=1, left=1, right=1 mean that 1 pixel-wide border needs to be built.
 * @param bottom: see top
 * @param left: see top
 * @param right: see top
 * @param borderType:
 * @param value: Border value if borderType==BORDER_CONSTANT.
 */
dsa::CopyMakeBorderFilter::CopyMakeBorderFilter(int top, int bottom, int left, int right, int borderType, const cv::Scalar& value) {
	top_= top;
	bottom_ = bottom;
	left_ = left;
	right_ = right;
	borderType_ = borderType;
	value_ = value;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::CopyMakeBorderFilter::~CopyMakeBorderFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::CopyMakeBorderFilter::init() {
	top = &top_;
	bottom = &bottom_;
	left = &left_;
	right = &right_;
	borderType = &borderType_;
	value = &value_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::CopyMakeBorderFilter::apply() {
	if(isReady()) {
		cv::copyMakeBorder(*src, dst, *top, *bottom, *left, *right, *borderType, *value);
	}
}

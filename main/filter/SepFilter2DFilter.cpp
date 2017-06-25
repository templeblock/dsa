/*
 * SepFilter2DFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#sepfilter2d
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "SepFilter2DFilter.h"

/**
 * Constructor
 *
 * @param ddepth: Destination image depth.
 * @param rowKernel: Coefficients for filtering each row.
 * @param columnKernel: Coefficients for filtering each column.
 * @param anchor: Anchor position within the kernel. The default value (-1, 1) means that the anchor is at the kernel center.
 * @param delta: Value added to the filtered results before storing them.
 * @param borderType: Pixel extrapolation method.
 */
dsa::SepFilter2DFilter::SepFilter2DFilter(int ddepth, cv::Mat& rowKernel, cv::Mat& columnKernel, cv::Point anchor, double delta, int borderType) {
	ddepth_ = ddepth;
	rowKernel_ = rowKernel;
	columnKernel_ = columnKernel;
	anchor_ = anchor;
	delta_ = delta;
	borderType_ = borderType;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::SepFilter2DFilter::~SepFilter2DFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::SepFilter2DFilter::init() {
	ddepth = &ddepth_;
	rowKernel = &rowKernel_;
	columnKernel = &columnKernel_;
	anchor = &anchor_;
	delta = &delta_;
	borderType = &borderType_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::SepFilter2DFilter::apply() {
	if(isReady()) {
		cv::sepFilter2D(*src, dst, *ddepth, *rowKernel, *columnKernel, *anchor, *delta, *borderType);
	}
}

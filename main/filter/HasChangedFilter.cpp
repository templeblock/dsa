/*
 * HasChangedFilter.cpp
 *
 * 	Determine whether there was a significant change compared to the previous image. A significant change is a change in a number of pixels that represent a ratio of the image greater than a given threshold.
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "HasChangedFilter.h"
#include <iostream>

/**
 * Constructor
 *
 */
dsa::HasChangedFilter::HasChangedFilter(const double changeRatioThreshold) : ImageBufferFilter(1) {
	changeRatioThreshold_ = changeRatioThreshold;
	hasChanged_ = false;
	hasChanged = &hasChanged_;
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::HasChangedFilter::~HasChangedFilter() {
}

/**
 * Possible optimizations:
 * - create a filter to avoid direct pixel access and make the operations in the filter window instead
 * - determine mean.val[] threshold dynamically or find a better value
 *
 */
void dsa::HasChangedFilter::evaluate() {
	if(buffer_.isEmpty())
		hasChanged_ = false;
	else {
		cv::Mat diff;
		absdiff(*src, *(buffer_.read()), diff);
		double diffPercent = countNonZero(diff)/(src->rows*src->cols);
		hasChanged_ = (diffPercent > changeRatioThreshold_) ? true : false;
	}
	ImageBufferFilter::evaluate();
}

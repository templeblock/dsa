/*
 * FlipFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#void%20flip%28InputArray%20src,%20OutputArray%20dst,%20int%20flipCode%29
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "FlipFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::FlipFilter::FlipFilter(int flipCode) {
	flipCode_ = flipCode;
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::FlipFilter::~FlipFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::FlipFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::FlipFilter::apply() {
	if(isReady()) {
		flip(*src, dst, flipCode_);
	}
}

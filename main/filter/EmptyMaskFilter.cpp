/*
 * EmptyMaskFilter.cpp
 *
 * Create an empty mask (black image) of the same size as input image
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "EmptyMaskFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::EmptyMaskFilter::EmptyMaskFilter() {
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::EmptyMaskFilter::~EmptyMaskFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::EmptyMaskFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::EmptyMaskFilter::apply() {
	if(isReady()) {
		dst = cv::Mat::zeros(src->rows, src->cols, CV_8UC1);
	}
}

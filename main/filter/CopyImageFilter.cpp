/*
 * CopyImageFilter.h
 *
 * Provides copies of its input images as output
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#include "CopyImageFilter.h"

/**
 * Constructor
 *
 * @param code: Color space conversion code.
 * @param dstCn: Number of channels in the destination image. If the parameter is 0, the number of the channels is derived automatically from src and code.
 */
dsa::CopyImageFilter::CopyImageFilter() {
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::CopyImageFilter::~CopyImageFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::CopyImageFilter::init() {
}

/**
 * When filter is ready, create copy of input image and provide it as output.
 */
void dsa::CopyImageFilter::apply() {
	if(isReady()) {
		dst = src->clone();
	}
}

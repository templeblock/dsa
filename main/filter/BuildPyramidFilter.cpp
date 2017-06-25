/*
 * buildPyramidFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#buildpyramid
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "BuildPyramidFilter.h"

/**
 * Constructor
 *
 * @param maxlevel: 0-based index of the last (the smallest) pyramid layer. It must be non-negative.
 */
dsa::BuildPyramidFilter::BuildPyramidFilter(int maxlevel) {
	maxlevel_ = maxlevel;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BuildPyramidFilter::~BuildPyramidFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BuildPyramidFilter::init() {
	maxlevel = &maxlevel_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::BuildPyramidFilter::apply() {
	if(isReady()) {
		buildPyramid(*src, dst, *maxlevel);
	}
}

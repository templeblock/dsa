/*
 * DrawFilter.cpp
 *
 *	BaseClass to draw shapes to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "DrawFilter.h"

/**
 * Constructor
 *
 * @param color: color to draw them in
 * @param everyNth: only do this every n'th frame
 */
dsa::DrawFilter::DrawFilter(cv::Scalar color, UINT32 everyNth) {
	everyNth_ = everyNth;
	color_ = color;
	trigger_ = false;
	run_ = 0;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::DrawFilter::~DrawFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::DrawFilter::init() {
	trigger = &trigger_;
	everyNth = &everyNth_;
	color = &color_;
}

/**
 * When filter is ready, draw if criteria are met.
 */
void dsa::DrawFilter::apply() {
	if(isReady()) {
		dst = src->clone();
		if((run_ % everyNth_ == 0) || trigger_) {
			trigger_ = false;
			draw();
		}
		run_++;
	}
}

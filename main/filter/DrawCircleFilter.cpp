/*
 * DrawCircleFilter.h
 *
 *	Draw a number of circles to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "DrawCircleFilter.h"

/**
 * Constructor
 *
 * @param circles: vector of Circles to draw. Each circle is described by its center point and radius
 * @param color: the color to draw them in
 * @param everyNth: only do this every n'th frame
 */
dsa::DrawCircleFilter::DrawCircleFilter(std::vector<Circle>& circles, cv::Scalar color, UINT32 everyNth) : dsa::DrawFilter(color, everyNth) {
	circles_ = circles;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::DrawCircleFilter::~DrawCircleFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::DrawCircleFilter::init() {
	circles = &circles_;
}

/**
 * When filter is ready, draw if criteria are met.
 */
void dsa::DrawCircleFilter::draw() {
	for(size_t i = 0; i < circles->size(); i++) {
		cv::circle(dst, (*circles)[i].center, (*circles)[i].radius, *color, -1);
	}
}

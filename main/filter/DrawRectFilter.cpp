/*
 * DrawRectFilter.h
 *
 *	Draw a number of rectangles to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "DrawRectFilter.h"

/**
 * Constructor
 *
 * @param rectangles: vector of rectangles to draw
 * @param color: color to draw them in
 * @param everyNth: only do this every n'th frame
 */
dsa::DrawRectFilter::DrawRectFilter(std::vector<cv::Rect>& rectangles, cv::Scalar color, UINT32 everyNth) : DrawFilter(color, everyNth) {
	rectangles_ = rectangles;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::DrawRectFilter::~DrawRectFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::DrawRectFilter::init() {
	rectangles = &rectangles_;
}

/**
 * When filter is ready, draw if criteria are met.
 */
void dsa::DrawRectFilter::draw() {
	for(size_t i = 0; i < rectangles->size(); i++) {
		cv::rectangle(dst, (*rectangles)[i], *color);
	}
}

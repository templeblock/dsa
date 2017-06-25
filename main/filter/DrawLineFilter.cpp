/*
 * DrawLineFilter.h
 *
 *	Draw a number of lines to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "DrawLineFilter.h"

/**
 * Constructor
 *
 * @param lines: vector of pairs of points (later are stored in a vec4i) to describe the lines between them. The first two coordinates in the vector are x,y of the first point, last two coordinates are x,y of the second point
 * @param color: the color to draw them in
 * @param everyNth: only do this every n'th frame
 */
dsa::DrawLineFilter::DrawLineFilter(std::vector<cv::Vec4i>& lines, cv::Scalar color, UINT32 everyNth) : DrawFilter(color, everyNth) {
	lines_ = lines;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::DrawLineFilter::~DrawLineFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::DrawLineFilter::init() {
	lines = &lines_;
}

/**
 * When filter is ready, draw if criteria are met.
 */
void dsa::DrawLineFilter::draw() {
	for(size_t i = 0; i < lines->size(); i++) {
		cv::line(dst, cv::Point((*lines)[i][0], (*lines)[i][1]), cv::Point((*lines)[i][2], (*lines)[i][3]), *color, 4);
	}
}

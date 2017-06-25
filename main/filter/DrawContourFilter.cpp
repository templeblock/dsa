/*
 * DrawContourFilter.h
 *
 *	Draw contours to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "DrawContourFilter.h"

/**
 * Constructor
 *
 * @param contours: the contours to draw. You can obtain such a list by running findContours or FindContourFilter
 * @param color: color to draw them in
 * @param everyNth: only do this every n'th frame
 */
dsa::DrawContourFilter::DrawContourFilter(std::vector<std::vector<cv::Point> >& contours, cv::Scalar color, UINT32 everyNth) : DrawFilter(color, everyNth) {
	contours_ = contours;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::DrawContourFilter::~DrawContourFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::DrawContourFilter::init() {
	contours = &contours_;
}

/**
 * When filter is ready, draw if criteria are met.
 */
void dsa::DrawContourFilter::draw() {
	cv::drawContours( dst, *contours, -1, *color);
}

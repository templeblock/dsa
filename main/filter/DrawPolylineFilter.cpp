/*
 * DrawPolylineFilter.h
 *
 *	Draw a number of polylines to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "DrawPolylineFilter.h"

/**
 * Constructor
 *
 * @param polylines: vector of sequences of points describing the polylines to draw
 * @param color: the color to draw them in
 * @param everyNth: only do this every n'th frame
 */
dsa::DrawPolylineFilter::DrawPolylineFilter(std::vector<cv::Mat> polylines, cv::Scalar color, UINT32 everyNth) : DrawFilter(color, everyNth) {
	polylines_ = polylines;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::DrawPolylineFilter::~DrawPolylineFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::DrawPolylineFilter::init() {
	polylines = &polylines_;
}

/**
 * When filter is ready, draw if criteria are met.
 */
void dsa::DrawPolylineFilter::draw() {
	cv::polylines(dst, *polylines, true, *color);
}

/**
 * Update Polylines (draw different ones in the future)
 *
 * @param polylines
 */
void dsa::DrawPolylineFilter::setPolylines(std::vector<cv::Mat> polylines) {
	polylines_ = polylines;
	this->polylines = &polylines_;
}

/*
 * RoiFilter.h
 *
 * 	Extract a region of interest defined by a rectangle.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "RoiFilter.h"

/**
 * Constructor
 *
 * @param rect: Rectangle defining the region of interest
 */
dsa::RoiFilter::RoiFilter(cv::Rect rect) {
	roi_ = rect;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::RoiFilter::~RoiFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::RoiFilter::init() {
	roi = &roi_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::RoiFilter::apply() {
	if(isReady()) {
		dst = cv::Mat(*src, *roi).clone();
	}
}

/**
 * Update the ROI
 *
 * @param rect
 */
void dsa::RoiFilter::setRoi(cv::Rect rect) {
	roi_ = rect;
}

bool dsa::RoiFilter::isReady() const {
	return(ImageFilter::isReady() && roi->height > 0 && roi->width > 0);
}

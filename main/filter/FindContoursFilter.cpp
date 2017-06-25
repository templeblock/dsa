/*
 * FindContoursFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html#findcontours
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "FindContoursFilter.h"

/**
 * Constructor
 *
 * @param contours: Detected contours. Each contour is stored as a vector of points.
 * @param mode: Contour retrieval mode
 * @param method: Contour approximation method
 * @param offset: Optional offset by which every contour point is shifted. This is useful if the contours are extracted from the image ROI and then they should be analyzed in the whole image context.
 */
dsa::FindContoursFilter::FindContoursFilter(std::vector<std::vector<cv::Point> > &contours, int mode, int method, cv::Point offset) {
	contours_ = contours;
	mode_ = mode;
	method_ = method;
	offset_ = offset;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::FindContoursFilter::~FindContoursFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::FindContoursFilter::init() {
	contours = &contours_;
	mode = &mode_;
	method = &method_;
	offset = &offset_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::FindContoursFilter::evaluate() {
	cv::findContours(*src, *contours, *mode, *method, *offset);
}

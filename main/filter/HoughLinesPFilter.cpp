/*
 * HoughLinesPFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlinesp
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "HoughLinesPFilter.h"

/**
 * Constructor
 *
 * @param lines: Output vector of lines.
 * @param rho: Distance resolution of the accumulator in pixels.
 * @param theta: Angle resolution of the accumulator in radians.
 * @param threshold: Accumulator threshold parameter.
 * @param minLineLength: Minimum line length. Line segments shorter than that are rejected.
 * @param maxLineGap: Maximum allowed gap between points on the same line to link them.
 */
dsa::HoughLinesPFilter::HoughLinesPFilter(cv::Mat& lines, double rho, double theta, int threshold, double minLineLength, double maxLineGap) {
	lines_ = lines;
	rho_ = rho;
	theta_ = theta;
	threshold_ = threshold;
	minLineLength_ = minLineLength;
	maxLineGap_ = maxLineGap;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::HoughLinesPFilter::~HoughLinesPFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::HoughLinesPFilter::init() {
	lines = &lines_;
	rho = &rho_;
	theta = &theta_;
	threshold = &threshold_;
	minLineLength = &minLineLength_;
	maxLineGap = &maxLineGap_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::HoughLinesPFilter::evaluate() {
	HoughLinesP(*src, *lines, *rho, *theta, *threshold, *minLineLength, *maxLineGap);
}

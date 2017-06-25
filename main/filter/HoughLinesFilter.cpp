/*
 * HoughLinesFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlines
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "HoughLinesFilter.h"

/**
 * Constructor
 *
 * @param lines: Output vector of lines.
 * @param rho: Distance resolution of the accumulator in pixels.
 * @param theta: Angle resolution of the accumulator in radians.
 * @param threshold: Accumulator threshold parameter. Only those lines are returned that get enough votes (> threshold).
 * @param srn: For the multi-scale Hough transform, it is a divisor for the distance resolution rho . The coarse accumulator distance resolution is rho and the accurate accumulator resolution is rho/srn . If both srn=0 and stn=0 , the classical Hough transform is used. Otherwise, both these parameters should be positive.
 * @param stn: For the multi-scale Hough transform, it is a divisor for the distance resolution theta.
 */
dsa::HoughLinesFilter::HoughLinesFilter(cv::Mat& lines, double rho, double theta, int threshold, double srn, double stn) {
	lines_ = lines;
	rho_ = rho;
	theta_ = theta;
	threshold_ = threshold;
	srn_ = srn;
	stn_ = stn;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::HoughLinesFilter::~HoughLinesFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::HoughLinesFilter::init() {
	lines = &lines_;
	rho = &rho_;
	theta = &theta_;
	threshold = &threshold_;
	srn = &srn_;
	stn = &stn_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::HoughLinesFilter::evaluate() {
	HoughLines(*src, *lines, *rho, *theta, *threshold, *srn, *stn);
}

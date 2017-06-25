/*
 * HoughCirclesFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghcircles
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "HoughCirclesFilter.h"

/**
 * Constructor
 *
 * @param circles: Output vector of found circles. Each vector is encoded as a 3-element floating-point vector (x, y, radius).
 * @param method: Detection method to use. Currently, the only implemented method is CV_HOUGH_GRADIENT , which is basically 21HT , described in [Yuen90].
 * @param dp: Inverse ratio of the accumulator resolution to the image resolution. For example, if dp=1 , the accumulator has the same resolution as the input image. If dp=2 , the accumulator has half as big width and height.
 * @param minDist: Minimum distance between the centers of the detected circles. If the parameter is too small, multiple neighbor circles may be falsely detected in addition to a true one. If it is too large, some circles may be missed.
 * @param param1: First method-specific parameter. In case of CV_HOUGH_GRADIENT , it is the higher threshold of the two passed to the Canny() edge detector (the lower one is twice smaller).
 * @param param2: Second method-specific parameter. In case of CV_HOUGH_GRADIENT , it is the accumulator threshold for the circle centers at the detection stage. The smaller it is, the more false circles may be detected. Circles, corresponding to the larger accumulator values, will be returned first.
 * @param minRadius: Minimum circle radius.
 * @param maxRadius: Maximum circle radius.
 */
dsa::HoughCirclesFilter::HoughCirclesFilter(cv::Mat& circles, int method, double dp, double minDist, double param1, double param2, int minRadius, int maxRadius) {
	circles_ = circles;
	method_ = method;
	dp_ = dp;
	minDist_ = minDist;
	param1_ = param1;
	param2_ = param2;
	minRadius_ = minRadius;
	maxRadius_ = maxRadius;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::HoughCirclesFilter::~HoughCirclesFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::HoughCirclesFilter::init() {
	circles = &circles_;
	method = &method_;
	dp = &dp_;
	minDist = &minDist_;
	param1 = &param1_;
	param2 = &param2_;
	minRadius = &minRadius_;
	maxRadius = &maxRadius_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::HoughCirclesFilter::evaluate() {
	HoughCircles(*src, *circles, *method, *dp, *minDist, *param1, *param2, *minRadius, *maxRadius);
}

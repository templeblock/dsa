/*
 * CornerSubPixFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#cornersubpix
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "CornerSubPixFilter.h"

/**
 * Constructor
 *
 * @param corners: Initial coordinates of the input corners and refined coordinates provided for output.
 * @param winSize: Half of the side length of the search window. For example, if winSize=Size(5,5), then a 5*2+1 x 5*2+1 = 11 x 11 search window is used.
 * @param zeroZone: Half of the size of the dead region in the middle of the search zone over which the summation in the formula below is not done. It is used sometimes to avoid possible singularities of the autocorrelation matrix. The value of (-1,-1) indicates that there is no such a size.
 * @param criteria: Criteria for termination of the iterative process of corner refinement. That is, the process of corner position refinement stops either after criteria.maxCount iterations or when the corner position moves by less than criteria.epsilon on some iteration.
 */
dsa::CornerSubPixFilter::CornerSubPixFilter(cv::Mat& corners, cv::Size winSize, cv::Size zeroZone, cv::TermCriteria& criteria) {
	corners_ = corners;
	winSize_ = winSize;
	zeroZone_ = zeroZone;
	criteria_ = criteria;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::CornerSubPixFilter::~CornerSubPixFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::CornerSubPixFilter::init() {
	corners = &corners_;
	winSize = &winSize_;
	zeroZone = &zeroZone_;
	criteria = &criteria_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::CornerSubPixFilter::evaluate() {
	cornerSubPix(*src, *corners, *winSize, *zeroZone, *criteria);
}

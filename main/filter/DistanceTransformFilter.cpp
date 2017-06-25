/*
 * DistanceTransformFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform
 *
 *	for details on the image filter.
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#include "DistanceTransformFilter.h"

/**
 * Constructor
 *
 * @param labels: Optional output 2D array of labels (the discrete Voronoi diagram). It has the type CV_32SC1 and the same size as src.
 * @param distanceType: Type of distance. It can be CV_DIST_L1, CV_DIST_L2 , or CV_DIST_C.
 * @param maskSize: Size of the distance transform mask. It can be 3, 5, or CV_DIST_MASK_PRECISE.
 */
dsa::DistanceTransformFilter::DistanceTransformFilter(cv::Mat &labels, int distanceType, int maskSize) {
	labels_ = labels;
	distanceType_ = distanceType;
	maskSize_ = maskSize;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::DistanceTransformFilter::~DistanceTransformFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::DistanceTransformFilter::init() {
	 labels = &labels_;
	 distanceType = &distanceType_;
	 maskSize = &maskSize_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::DistanceTransformFilter::evaluate() {
	cv::distanceTransform(*src, dst, *labels, *distanceType, *maskSize);
}

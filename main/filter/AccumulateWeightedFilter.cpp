/*
 * AccumulateWeightedFiltere.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/motion_analysis_and_object_tracking.html#accumulateweighted
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "AccumulateWeightedFilter.h"

/**
 * Constructor
 *
 * @param alpha: Weight of the input image.
 * @param mask: Optional operation mask.
 */
dsa::AccumulateWeightedFilter::AccumulateWeightedFilter(double alpha, const cv::Mat& mask) {
	alpha_ = alpha;
	mask_ = mask;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::AccumulateWeightedFilter::~AccumulateWeightedFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::AccumulateWeightedFilter::init() {
	alpha = &alpha_;
	mask = &mask_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::AccumulateWeightedFilter::apply() {
	if(isReady()) {
		cv::accumulateWeighted(*src, dst, *alpha, *mask);
	}
}

/*
 * NormalizeFilter.h
 *
 *  See
 *
 *	http://opencv.willowgarage.com/documentation/cpp/operations_on_arrays.html#cv-normalize
 *
 *	for details on the image filter
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "NormalizeFilter.h"

/**
 * Constructor
 *
 * @param alpha: The norm value to normalize to or the lower range boundary in the case of range normalization
 * @param beta: The upper range boundary in the case of range normalization; not used for norm normalization
 * @param normType: The normalization type, see the discussion
 * @param rtype: When the parameter is negative, the destination array will have the same type as src , otherwise it will have the same number of channels as src and the depth =CV_MAT_DEPTH(rtype)
 * @param mask: The optional operation mask
 */
dsa::NormalizeFilter::NormalizeFilter(double alpha, double beta, int normType, int rtype, const cv::Mat& mask) {
	alpha_ = alpha;
	beta_ = beta;
	normType_ = normType;
	rtype_ = rtype;
	mask_ = mask;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::NormalizeFilter::~NormalizeFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::NormalizeFilter::init() {
	alpha = &alpha_;
	beta = &beta_;
	normType = &normType_;
	rtype = &rtype_;
	mask = &mask_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::NormalizeFilter::apply() {
	if(isReady()) {
		cv::normalize(*src, dst, *alpha, *beta, *normType, *rtype, *mask);
	}
}

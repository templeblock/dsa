/*
 * SubtractReferenceFilter.cpp
 *
 * Subtract a reference image from another image of the same size
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "SubtractReferenceFilter.h"

/**
 * Constructor
 *
 * @param mask: Optional operation mask.
 */
dsa::SubtractReferenceFilter::SubtractReferenceFilter(const cv::Mat& reference, const cv::Mat& mask) {
	reference_ = reference;
	mask_ = mask;
	referenceMean_ = cv::mean(reference_, mask_);

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::SubtractReferenceFilter::~SubtractReferenceFilter() {

}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::SubtractReferenceFilter::init() {
	reference = &reference_;
	mask = &mask_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::SubtractReferenceFilter::apply() {
	if(isReady()) {
		double factor = cv::mean(*src, mask_).val[0]/referenceMean_.val[0];
		cv::Mat tmpref = factor*(reference->clone());
		//imshow("ReferenzFaktor", tmpref);
		cv::absdiff(*src, tmpref, dst);
	}
}

/**
 * Update Reference image and mask
 *
 * @param reference
 * @param mask
 */
void dsa::SubtractReferenceFilter::setReference(const cv::Mat& reference, const cv::Mat& mask) {
	reference_ = reference;
	mask_ = mask;
	referenceMean_ = cv::mean(reference_);
}

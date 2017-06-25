/*
 * HOGFilter.cpp
 *
 * 	Compute HOG Descriptor for images
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "HOGFilter.h"

/**
 * Constructor
 *
 */
dsa::HOGFilter::HOGFilter(cv::Size winSize) {
	dSize_ = winSize_.height * winSize_.width;
	winSize_ = winSize;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::HOGFilter::~HOGFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::HOGFilter::init() {
	EvaluationFilter::init();
}

/**
 * Compute HOG values for all segments
 *
 */
void dsa::HOGFilter::evaluate() {
	descriptorValues.release();	// clear old values
	descriptorValues = cv::Mat(segments->size(), dSize_, CV_32FC1);	// new matrix

	for(size_t i = 0; i < segments->size(); i++) {	// for all segments
		std::vector<float> descriptor = std::vector<float>();

		if(segments->at(i).rows == winSize_.height && segments->at(i).cols == winSize_.width) {	// correct size
			hog_.compute(segments->at(i), descriptor, winSize_, cv::Size());
		} else { // resize first
			cv::Mat segment;
			cv::resize(segments->at(i), segment, winSize_, 0, 0);
			hog_.compute(segment, descriptor, winSize_, cv::Size());
		}

		// copy into output matrix
		for(size_t j = 0; j < descriptor.size(); j++) {
			descriptorValues.at<float>(i,j) = descriptor[j];
		}
	}
}

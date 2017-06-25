/*
 * TrainingDataGathererFilter.cpp
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "TrainingDataGathererFilter.h"

/**
 * Constructor
 *
 */
dsa::TrainingDataGathererFilter::TrainingDataGathererFilter(const std::string &filename, const UINT32 maxElements, const UINT32 everyNth) {
	everyNth_ = everyNth;
	tdm_ = new TrainingDataManager<std::vector<float> >(filename, maxElements);
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::TrainingDataGathererFilter::~TrainingDataGathererFilter() {
	delete tdm_;
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::TrainingDataGathererFilter::init() {
	EvaluationFilter::init();
}

/**
 * Add to training data
 *
 */
void dsa::TrainingDataGathererFilter::evaluate() {
	cv::HOGDescriptor hog;
	std::vector<float> descriptor = std::vector<float>();
	std::vector<cv::Point> locations;
	hog.compute(*src, descriptor, cv::Size(32,32), cv::Size(0,0), locations);	// compute hog features

	if(!tdm_->isFull() || run_%everyNth_ == 0)
		tdm_->add(descriptor);
	run_++;
	// create matrix from descriptor vector for use in training
	/*cv::Mat traininfo(descriptor.size(), 1, CV_32FC1);
	for(size_t i = 0; i < descriptor.size(); i++)
		traininfo.at<float>(i,0) = descriptor.at(i);*/
}

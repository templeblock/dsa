/*
 * BayesModelFilter.cpp
 *
 * 	Normal bayes model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "BayesModelFilter.h"

/**
 * Constructor
 *
 */
dsa::BayesModelFilter::BayesModelFilter(const char* filename, const char* name) : StatModelFilter(filename, name) {
	type_ = MLType::NB;
	bayes_.load(filename, name);

	init();
}

/**
 * Destructor
 *
 * Save the settings.
 */
dsa::BayesModelFilter::~BayesModelFilter() {
}

/**
 * Run predict() on the MLP
 */
void dsa::BayesModelFilter::predict() {


	// alternative: one at a time
	/*for(size_t i = 0; i < descriptorValues->rows; i++) {
		cv::Mat output(1, 1, CV_32FC1);
		output.at<float>(0,0) = bayes_.predict(descriptorValues->row(i));
		results.push_back(GenericMLResult(type_, output));
	}*/
}

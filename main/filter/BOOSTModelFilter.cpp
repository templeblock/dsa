/*
 * BOOSTModelFilter.cpp
 *
 * 	BOOST model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "BOOSTModelFilter.h"

/**
 * Constructor
 *
 */
dsa::BOOSTModelFilter::BOOSTModelFilter(const char* filename, const char* name) : StatModelFilter(filename, name) {
	type_ = MLType::BOOST;
	boost_.load(filename, name);

	init();
}

/**
 * Destructor
 *
 * Save the settings.
 */
dsa::BOOSTModelFilter::~BOOSTModelFilter() {
}

/**
 * Run predict() on the MLP
 */
void dsa::BOOSTModelFilter::predict() {
	for(size_t i = 0; i < descriptorValues->rows; i++) {
		cv::Mat output(1, 1, CV_32FC1);
		output.at<float>(1,1) = boost_.predict(descriptorValues->row(i));
		results.push_back(GenericMLResult(type_, output));
	}
}

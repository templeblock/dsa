/*
 * DTModelFilter.cpp
 *
 * 	Decision tree model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "DTModelFilter.h"

/**
 * Constructor
 *
 */
dsa::DTModelFilter::DTModelFilter(const char* filename, const char* name) : StatModelFilter(filename, name) {
	type_ = MLType::DT;
	tree_.load(filename, name);

	init();
}

/**
 * Destructor
 *
 * Save the settings.
 */
dsa::DTModelFilter::~DTModelFilter() {
}

/**
 * Run predict() on the MLP
 */
void dsa::DTModelFilter::predict() {
	for(size_t i = 0; i < descriptorValues->rows; i++) {
		cv::Mat output(1, 1, CV_32FC1);
		output.at<float>(0,0) = tree_.predict(descriptorValues->row(i))->value;
		results.push_back(GenericMLResult(type_, output));
	}
}

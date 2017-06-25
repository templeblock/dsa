/*
 * GBTModelFilter.cpp
 *
 * 	Gradient Boosted tree model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "GBTModelFilter.h"

/**
 * Constructor
 *
 */
dsa::GBTModelFilter::GBTModelFilter(const char* filename, const char* name) : StatModelFilter(filename, name) {
	type_ = MLType::GBT;
	gbt_.load(filename, name);

	init();
}

/**
 * Destructor
 *
 * Save the settings.
 */
dsa::GBTModelFilter::~GBTModelFilter() {
}

/**
 * Run predict() on the MLP
 */
void dsa::GBTModelFilter::predict() {
	for(size_t i = 0; i < descriptorValues->rows; i++) {
		cv::Mat output(1, 1, CV_32FC1);
		output.at<float>(0,0) = gbt_.predict(descriptorValues->row(i));
		results.push_back(GenericMLResult(type_, output));
	}
}

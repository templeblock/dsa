/*
 * RTModelFilter.cpp
 *
 * 	Random trees model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "RTModelFilter.h"

/**
 * Constructor
 *
 */
dsa::RTModelFilter::RTModelFilter(const char* filename, const char* name) : StatModelFilter(filename, name) {
	type_ = MLType::RT;
	rts_.load(filename, name);

	init();
}

/**
 * Destructor
 *
 * Save the settings.
 */
dsa::RTModelFilter::~RTModelFilter() {
}

/**
 * Run predict() on the MLP
 */
void dsa::RTModelFilter::predict() {
	for(size_t i = 0; i < descriptorValues->rows; i++) {
		cv::Mat output(1, 1, CV_32FC1);
		output.at<float>(0,0) = rts_.predict(descriptorValues->row(i));
		results.push_back(GenericMLResult(type_, output));
	}
}

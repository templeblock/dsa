/*
 * ERTModelFilter.cpp
 *
 * 	Extremely randomized trees model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "ERTModelFilter.h"

/**
 * Constructor
 *
 */
dsa::ERTModelFilter::ERTModelFilter(const char* filename, const char* name) : StatModelFilter(filename, name) {
	type_ = MLType::ERT;
	erts_.load(filename, name);

	init();
}

/**
 * Destructor
 *
 * Save the settings.
 */
dsa::ERTModelFilter::~ERTModelFilter() {
}

/**
 * Run predict() on the MLP
 */
void dsa::ERTModelFilter::predict() {
	for(size_t i = 0; i < descriptorValues->rows; i++) {
		cv::Mat output(1, 1, CV_32FC1);
		output.at<float>(0,0) = erts_.predict(descriptorValues->row(i));
		results.push_back(GenericMLResult(type_, output));
	}
}

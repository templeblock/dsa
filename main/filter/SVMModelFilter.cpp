/*
 * SVMModelFilter.cpp
 *
 * 	Support vector machine model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "SVMModelFilter.h"

/**
 * Constructor
 *
 */
dsa::SVMModelFilter::SVMModelFilter(const char* filename, const char* name) : StatModelFilter(filename, name) {
	type_ = MLType::SVM;
	svm_.load(filename, name);

	init();
}

/**
 * Destructor
 *
 * Save the settings.
 */
dsa::SVMModelFilter::~SVMModelFilter() {
}

/**
 * Run predict() on the MLP
 */
void dsa::SVMModelFilter::predict() {
	cv::Mat output;
	svm_.predict(*descriptorValues, output);
	for(size_t i = 0; i < descriptorValues->rows; i++) {
		results.push_back(GenericMLResult(type_, output.row(i)));
	}

	// alternative: one at a time
	/*for(size_t i = 0; i < descriptorValues->rows; i++) {
		cv::Mat output(1, 1, CV_32FC1);
		output.at<float>(0,0) = svm_.predict(descriptorValues->row(i));
		results.push_back(GenericMLResult(type_, output));
	}*/
}

/*
 * KNearestModelFilter.cpp
 *
 * 	KNearest neighbor model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "KNearestModelFilter.h"

/**
 * Constructor
 *
 */
dsa::KNearestModelFilter::KNearestModelFilter(const char* filename, const char* name, int K) : StatModelFilter(filename, name) {
	K_ = K;
	type_ = MLType::KNN;
	knn_.load(filename, name);

	init();
}

/**
 * Destructor
 *
 * Save the settings.
 */
dsa::KNearestModelFilter::~KNearestModelFilter() {
}

/**
 * Run predict() on the MLP
 */
void dsa::KNearestModelFilter::predict() {
	cv::Mat output(1, descriptorValues->rows, CV_32FC1);
	knn_.find_nearest(*descriptorValues, K_, &output);
	for(size_t i = 0; i < descriptorValues->rows; i++) {
		results.push_back(GenericMLResult(type_, output.row(i)));
	}
}

/*
 * TargetAreaFilter.h
 *
 *	Determines target areas from a vector of values
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "TargetAreaFilter.h"

/**
 * Constructor
 */
dsa::TargetAreaFilter::TargetAreaFilter(MathVector<int> mathVector) {
	areas_ = mathVector;
	areas = &areas_;
}

/**
 * Destructor
 */
dsa::TargetAreaFilter::~TargetAreaFilter() {
}

void dsa::TargetAreaFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	figures.clear();
}

/**
 * Transform data representation from plain vector to circle struct
 */
void dsa::TargetAreaFilter::evaluate() {
	if(areas->size() % 3 == 0) {
		for(size_t i = 0; i < areas->size(); i+=3) {
			Circle nextCircle = Circle();
			nextCircle.center = cv::Point((*areas)[i], (*areas)[i+1]);
			nextCircle.radius = (*areas)[i+2];
			figures.push_back(nextCircle);
		}
	}
}

/*
 * StatModelFilter.cpp
 *
 * 	Base class for statistical models
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "StatModelFilter.h"

/**
 * Constructor
 *
 */
dsa::StatModelFilter::StatModelFilter(const char* filename, const char* name, bool autoLearn, float autoLearnThreshold) {
	filename_ = filename;
	name_ = name;
	autoLearn_ = autoLearn;
	autoLearnThreshold_ = autoLearnThreshold;

	results = std::vector<GenericMLResult>();

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::StatModelFilter::~StatModelFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::StatModelFilter::init() {
	EvaluationFilter::init();
}

bool dsa::StatModelFilter::isReady() {
	return(! descriptorValues->empty());
}

void dsa::StatModelFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	results.clear();
}

/**
 * Run predict() of subclass (subclasses are required to implement that method)
 */
void dsa::StatModelFilter::evaluate() {
	predict();
}

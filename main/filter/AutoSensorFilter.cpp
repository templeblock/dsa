/*
 * AutoSensorFilter.cpp
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "AutoSensorFilter.h"

/**
 * Constructor
 *
 */
dsa::AutoSensorFilter::AutoSensorFilter(AutoSensorAlgorithm* const autosensorAlgorithm, cv::Mat mask) {
	autosensorAlgorithm_ = autosensorAlgorithm;
	usemask_ = !mask.empty();
	mask_ = mask;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::AutoSensorFilter::~AutoSensorFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::AutoSensorFilter::init() {
	EvaluationFilter::init();

	usemask = &usemask_;
	mask = &mask_;
}

/**
 * Adjust sensor parameter where necessary
 *
 */
void dsa::AutoSensorFilter::evaluate() {
	if(*usemask) {
		autosensorAlgorithm_->run(*src, *mask);
	} else {
		autosensorAlgorithm_->run(*src);
	}
}

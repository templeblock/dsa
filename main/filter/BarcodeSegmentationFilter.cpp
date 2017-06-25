/*
 * BarcodeSegmentationFilter.cpp
 *
 * 	See
 *
 *	barcode_extraction_framework.pdf
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "BarcodeSegmentationFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::BarcodeSegmentationFilter::BarcodeSegmentationFilter(const int ksize) {
	BarcodeSegmenter_ = BarcodeSegmenter(ksize);
	candidates = std::vector<Roi>();

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BarcodeSegmentationFilter::~BarcodeSegmentationFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BarcodeSegmentationFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function. According to amdahl's law any significant speedup has to optimize candidatesFromComponents() method which eats 2/3 of this filters cpu consumption and 3/4 of the overall barcode demonstrator.
 */
void dsa::BarcodeSegmentationFilter::evaluate() {
	BarcodeSegmenter_.find(*src, candidates);
}

void dsa::BarcodeSegmentationFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	candidates.clear();
}

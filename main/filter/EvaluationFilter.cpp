/*
 * EvaluationFilter.cpp
 *
 *	Is essentially an ImageFilter, except that the input image itself is not changed in any way, but passed on
 *	for further consideration.
 *	An additional function named evaluate(), which subclasses have to implement is used to extract
 *	information from the image.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "EvaluationFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::EvaluationFilter::EvaluationFilter() {
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::EvaluationFilter::~EvaluationFilter() {
}

/**
 * We are required to implement it, but don't need the function, hence it's empty.
 */
void dsa::EvaluationFilter::init() {
}

/**
 * When filter is ready, run the evaluate function and make input image available as output.
 */
void dsa::EvaluationFilter::apply() {
	if(isReady()) {
		dst = *src;	// pass-though src, since we do not modify it (copies headers, references data)
		evaluate();
	}
}

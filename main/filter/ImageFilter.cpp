/*
 * ImageFilter.cpp
 *
 *	Common image filter class. The actual image filters are derived from this class and have to
 *	implement the apply() method which does the magic of the filter on the image.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "ImageFilter.h"

/**
 * Empty constructor.
 */
dsa::ImageFilter::ImageFilter() {
	src = 0;
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ImageFilter::~ImageFilter() {
}

/**
 * An ImageFilter is considered ready, when it's input image exists and has data
 *
 * @return true if we have a real image to process, false otherwise
 */
bool dsa::ImageFilter::isReady() const {
	return(src != 0 && src->data);
}

void dsa::ImageFilter::preprocessing() {
}

void dsa::ImageFilter::postprocessing() {
}

void dsa::ImageFilter::bypass() {
	if(isReady()) {
		dst = *src;	// pass-though src, since we do not modify it (copies headers, references data)
	}
}

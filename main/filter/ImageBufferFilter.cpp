/*
 * ImageBufferFilter.cpp
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "ImageBufferFilter.h"

/**
 * Constructor
 *
 * @param size: capacity of the buffer (number of elements max)
 */
dsa::ImageBufferFilter::ImageBufferFilter(const size_t size) {
	buffer_ = ImageBuffer(size);

	init();
}

/**
 * Destructor
 */
dsa::ImageBufferFilter::~ImageBufferFilter() {
}

/**
 * Write image into buffer.
 */
void dsa::ImageBufferFilter::evaluate() {
	buffer_.write(*src);
}

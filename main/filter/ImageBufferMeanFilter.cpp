/*
 * ImageBufferMeanFilter.cpp
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "ImageBufferMeanFilter.h"

/**
 * Constructor
 *
 * @param size: capacity of the buffer (number of elements max)
 */
dsa::ImageBufferMeanFilter::ImageBufferMeanFilter(const size_t size) : ImageBufferFilter(size) {
	initialized_ = false;
}

/**
 * Destructor
 */
dsa::ImageBufferMeanFilter::~ImageBufferMeanFilter() {
}

void dsa::ImageBufferMeanFilter::apply() {
	if(isReady())	// prevent pass-through
		evaluate();
}

/**
 * Write image into buffer.
 */
void dsa::ImageBufferMeanFilter::evaluate() {
	if(!initialized_) {
		sum_ = cv::Mat::zeros(src->rows, src->cols, CV_16UC1);
		initialized_ = true;
	}

	if(buffer_.isFull())
		subtract(sum_, *(buffer_.readFirst()), sum_, cv::noArray(), CV_16UC1);	// subtract image which will be evicted
	ImageBufferFilter::evaluate();	// write new image to buffer, evict in case of overflow
	add(*src, sum_, sum_, cv::noArray(), CV_16UC1);	// add new image to sum
	dst = cv::Mat(src->rows, src->cols, CV_8UC1);
	sum_.convertTo(dst, CV_8UC1, 1.0/buffer_.getSize());
}

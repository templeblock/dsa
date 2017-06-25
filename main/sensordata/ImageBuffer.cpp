/*
 * ImageBuffer.cpp
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "ImageBuffer.h"

dsa::ImageBuffer::ImageBuffer(const size_t size) {
	size_ = size;
	buffer_ = std::list<cv::Mat*>();
}

dsa::ImageBuffer::~ImageBuffer() {
	free();
}

void dsa::ImageBuffer::write(cv::Mat& element) {
	if(isFull()) {
		delete buffer_.back();
		buffer_.pop_back();
	}
	cv::Mat* copy = new cv::Mat(element.clone());
	buffer_.push_front(copy);
}

const cv::Mat* dsa::ImageBuffer::read() const {
	return(buffer_.front());
}

const cv::Mat* dsa::ImageBuffer::read(const size_t n) const {
	if(n < buffer_.size()) {
		std::_List_const_iterator<cv::Mat*> it = buffer_.begin();
		for(size_t i = 0; i < n; i++)
			it++;
		return(*it);
	} else
		return(NULL);
}

const cv::Mat* dsa::ImageBuffer::readFirst() const {
	return(buffer_.back());
}

std::vector<cv::Mat*>* dsa::ImageBuffer::readLastN(const size_t nElements) const {
	size_t n = std::min(nElements, buffer_.size());
	std::vector<cv::Mat*>* vec = new std::vector<cv::Mat*>(n);
	std::_List_const_iterator<cv::Mat*> it = buffer_.begin();
	for(size_t i = 0; i < n; i++) {
		vec->at(i) = *it;
		it++;
	}

	return(vec);
}

void dsa::ImageBuffer::free() {
	while(buffer_.size() > 0) {
		delete buffer_.back();
		buffer_.pop_back();
	}
}

void dsa::ImageBuffer::resize(const size_t size) {
	while(buffer_.size() > size) {
		buffer_.pop_back();
		this->size_ = size;
	}
}

bool dsa::ImageBuffer::isEmpty() const {
	return(getSize() == 0);
}

bool dsa::ImageBuffer::isFull() const {
	return(getSize() >= size_);
}

size_t dsa::ImageBuffer::getSize() const {
	return(buffer_.size());
}

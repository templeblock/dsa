/*
 * CameraReader.cpp
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#include "CameraReader.h"

dsa::CameraReader::CameraReader(cv::VideoCapture* const capture) {
	index_ = 0;
	capture_ = *capture;
}

dsa::CameraReader::~CameraReader() {
}

cv::Mat dsa::CameraReader::getCurrentFrame() {
	return(currentFrame_);
}

cv::Mat dsa::CameraReader::getNextFrame() {
	capture_ >> currentFrame_;
	index_++;
	return(currentFrame_);
}

dsa::UINT64 dsa::CameraReader::getCurrentFrameIndex() {
	return(index_);
}

dsa::UINT64 dsa::CameraReader::getNextFrameIndex() {
	return(index_ + 1);
}

/*
 * VideoReader.cpp
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#include "VideoReader.h"

dsa::VideoReader::VideoReader(const std::string &filename, const int startFrameIndex) {
	filename_ = filename;
	capture_ = cv::VideoCapture(filename);
	setFrameIndex(startFrameIndex);
}

dsa::VideoReader::VideoReader(const std::string &filename, const double positionRatio) {
	filename_ = filename;
	capture_ = cv::VideoCapture(filename);
	setPositionRatio(positionRatio);
}

dsa::VideoReader::~VideoReader() {
}

bool dsa::VideoReader::open(const std::string &filename, const int startFrameIndex) {
	if(filename.length() > 0 && filename_.compare(filename) != 0) {
		filename_ = filename;
		capture_ = cv::VideoCapture(filename_);
		capture_.release();
	}
	return(capture_.isOpened() && setFrameIndex(startFrameIndex));
}

bool dsa::VideoReader::isOpened() const {
	return(capture_.isOpened());
}

void dsa::VideoReader::close() {
	if(isOpened())
		capture_.release();
}

cv::Mat dsa::VideoReader::getCurrentFrame() {
	return(currentFrame_);
}

cv::Mat dsa::VideoReader::getNextFrame() {
	capture_ >> currentFrame_;
	return(currentFrame_);
}

std::string dsa::VideoReader::getFilename() {
	return(filename_);
}

void dsa::VideoReader::setFilename(const std::string &filename) {
	if(filename_.compare(filename) != 0)
		close();
	open(filename);
}

int dsa::VideoReader::getCurrentFrameIndex() {
	int index = getNextFrameIndex() - 1;
	return(index >= 0 ? index : -1);
}

int dsa::VideoReader::getNextFrameIndex() {
	return(capture_.isOpened() ? (int)(capture_.get(CV_CAP_PROP_POS_FRAMES)) : -1);
}

bool dsa::VideoReader::setFrameIndex(const UINT32 frameIndex) {
	return((frameIndex < capture_.get(CV_CAP_PROP_FRAME_COUNT)) ? capture_.set(CV_CAP_PROP_POS_FRAMES, (int)frameIndex) : false);
}

bool dsa::VideoReader::setPositionRatio(const double positionRatio) {
	return ((positionRatio > 0.0 && positionRatio < 1.0) ? capture_.set(CV_CAP_PROP_POS_AVI_RATIO, positionRatio) : false);
}

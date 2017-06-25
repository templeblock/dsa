/*
 * VideoWriter.cpp
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#include "VideoWriter.h"

dsa::VideoWriter::VideoWriter(const std::string &filename, int fourcc, double fps, bool isColor) {
	initialized_ = false;
	filename_ = filename;
	fourcc_ = fourcc;
	fps_ = fps;
	isColor_ = isColor;
}

dsa::VideoWriter::~VideoWriter() {
	writer_.release();
}

void dsa::VideoWriter::init() {
	cv::Size Size();
	initialized_ = 	writer_.open(filename_, fourcc_, fps_, currentFrame_.size(), isColor_);
}

bool dsa::VideoWriter::write() {
	if(currentFrame_.data != NULL) {	// check if we have data
		if(initialized_) {	// already initialized
			writer_.write(currentFrame_);
			//writer_ << currentFrame_; // alternativ
			return(true);
		} else { // initialize (first time)
			init();
			if(initialized_) { // init successful => write frame
				writer_.write(currentFrame_);
				return(true);
			} else	// init failed
				return(false);
		}
	} else
		return(false);	// cant write frame with no data
}

bool dsa::VideoWriter::write(cv::Mat &image) {
	currentFrame_ = image;
	return(write());
}

std::string dsa::VideoWriter::getFilename() const {
	return(filename_);
}

cv::Mat dsa::VideoWriter::getImage() const {
	return(currentFrame_);
}

void dsa::VideoWriter::setImage(cv::Mat &image) {
	currentFrame_ = image;
}

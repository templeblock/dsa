/*
 * TimedVideoWriterFilter.cpp
 *
 *  Created on: 11.04.2013
 *      Author: Bigontina
 */

#include "TimedVideoWriterFilter.h"

/**
 * Constructor
 *
 * @param filename: video file name to save.
 * @param fourcc: video codec to use.
 * @param fps: number of frames per second the video should be played back at.
 * @param isColor: is there more than one color channel?
 */
dsa::TimedVideoWriterFilter::TimedVideoWriterFilter(const std::string &filename, int fourcc, double fps, bool isColor) {
	filename_ = filename;
	fourcc_ = fourcc;
	fps_ = fps;
	isColor_ = isColor;
	msPerFrame_ = CLOCKS_PER_SEC/fps_;
	prevTime_ = -1;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::TimedVideoWriterFilter::~TimedVideoWriterFilter() {
}

/**
 * This is no ordinary image filter, instead we create the VideoWriter object to write the video file upon init.
 */
void dsa::TimedVideoWriterFilter::init() {
	recorder_ = VideoWriter(filename_, fourcc_, fps_, isColor_);
}

/**
 * When filter is ready, make input image available as output and write it to the videostream.
 */
void dsa::TimedVideoWriterFilter::evaluate() {
	float now = (float)clock();
	while(prevTime_ != -1 && prevTime_ + msPerFrame_*1.5 < now) {
		recorder_.write();
		prevTime_ += msPerFrame_;
	}
	recorder_.write(dst);
	if(prevTime_ == -1) prevTime_ = now;
	else prevTime_ += msPerFrame_;
}

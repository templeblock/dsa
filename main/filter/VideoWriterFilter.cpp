/*
 * VideoWriterFilter.cpp
 *
 * 	Writes a video file of the incoming images. Please note that this can only work, if all further incoming
 * 	images have the same properties as the first image, i.e same size and number of channels, hence you cannot.
 * 	change the properties of the videostream on-the-fly.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "VideoWriterFilter.h"

/**
 * Constructor
 *
 * @param filename: video file name to save.
 * @param fourcc: video codec to use.
 * @param fps: number of frames per second the video should be played back at.
 * @param isColor: is there more than one color channel?
 */
dsa::VideoWriterFilter::VideoWriterFilter(const std::string &filename, int fourcc, double fps, bool isColor) {
	filename_ = filename;
	fourcc_ = fourcc;
	fps_ = fps;
	isColor_ = isColor;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::VideoWriterFilter::~VideoWriterFilter() {
}

/**
 * This is no ordinary image filter, instead we create the VideoWriter object to write the video file upon init.
 */
void dsa::VideoWriterFilter::init() {
	recorder_ = VideoWriter(filename_, fourcc_, fps_, isColor_);
}

/**
 * When filter is ready, make input image available as output and write it to the videostream.
 */
void dsa::VideoWriterFilter::evaluate() {
	recorder_.write(dst);
}

/*
 * UncompressedVideoWriterFilter.cpp
 *
 * 	Writes a video file of the incoming images. Please note that this can only work, if all further incoming
 * 	images have the same properties as the first image, i.e same size and number of channels, hence you cannot.
 * 	change the properties of the videostream on-the-fly.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "UncompressedVideoWriterFilter.h"

/**
 * Constructor
 *
 * @param filename: video file name to save.
 * @param fps: number of frames per second the video should be played back at.
 */
dsa::UncompressedVideoWriterFilter::UncompressedVideoWriterFilter(std::string filename, double fps) {
	currentFileNr_ = 0;
	currentVideoSize_ = 0;
	filename_ = filename;
	fps_ = fps;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::UncompressedVideoWriterFilter::~UncompressedVideoWriterFilter() {
}

/**
 * This is no ordinary image filter, instead we create the VideoWriter object to write the video file upon init.
 */
void dsa::UncompressedVideoWriterFilter::init() {
	std::string filename = filename_;
	filename.append(intToString(currentFileNr_)).append(".avi");
	recorder_ = VideoWriter(filename, 0, fps_, true);
}

/**
 * When filter is ready, make input image available as output and write it to the videostream.
 */
void dsa::UncompressedVideoWriterFilter::evaluate() {
	int frameSize = (dst.rows*dst.cols*dst.channels()) >> 10;	// size in kilobytes
	if(currentVideoSize_ + frameSize <= MAX_VIDEO_SIZE) { // fits into current file
		currentVideoSize_ += frameSize;
	} else { // doesnt fit into current file => create new file
		currentVideoSize_ = frameSize;
		currentFileNr_++;
		init();	// create new recorder
	}
	recorder_.write(dst);
	//std::cout << "wrote frame to " << filename_ << std::endl;
}

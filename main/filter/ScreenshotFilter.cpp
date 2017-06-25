/*
 * ScreenshotFilter.cpp
 *
 * 	Writes an image to a file upon pressing of a button.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "ScreenshotFilter.h"

/**
 * Constructor
 *
 * @param filename: video file name to save.
 * @param fourcc: video codec to use.
 * @param fps: number of frames per second the video should be played back at.
 * @param isColor: is there more than one color channel?
 */
dsa::ScreenshotFilter::ScreenshotFilter(const std::string basename, const std::string extension, char key) {
	basename_ = basename;
	extension_ = extension;
	key_ = key;
	prevTimestamp_ = 0;
	run_ = 0;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ScreenshotFilter::~ScreenshotFilter() {
}

/**
 * This is no ordinary image filter, instead we create the VideoWriter object to write the video file upon init.
 */
void dsa::ScreenshotFilter::init() {
	writer_ = ImageWriter();
}

/**
 * When filter is ready, make input image available as output and write it to the videostream.
 */
void dsa::ScreenshotFilter::evaluate() {
	if(cv::waitKey(10) == key_) {
		clock_t timestamp = currentTimeOfDayMilliseconds();
		std::string filename = std::string(basename_);
		filename.append(intToString(timestamp));

		if(timestamp == prevTimestamp_) {
			filename.append("-");
			filename.append(intToString(run_));
			run_++;
		} else {
			prevTimestamp_ = timestamp;
			run_ = 0;
		}
		filename.append(".");
		filename.append(extension_);
		std::cerr << filename << std::endl;
		writer_.write(dst, filename);
	}
}

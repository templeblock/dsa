/*
 * ImageOutputFilter.cpp
 *
 *	Shows the input image without modifications in a window.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "ImageOutputFilter.h"

/**
 * Constructor
 *
 * @param windowName: Name (title) of the window which shows the image.
 */
dsa::ImageOutputFilter::ImageOutputFilter(const std::string &windowName) {
	windowName_ = windowName;
	init();
}

/**
 * Destructor
 *
 * Destroy the window showing the image.
 */
dsa::ImageOutputFilter::~ImageOutputFilter() {
	cv::destroyWindow(windowName_);
}

/**
 * This is not an image filter, as it only shows the image, so upon init we create the window to show the images in.
 */
void dsa::ImageOutputFilter::init() {
	cv::namedWindow(windowName_, CV_WINDOW_AUTOSIZE);
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::ImageOutputFilter::evaluate() {
	cv::imshow(windowName_, *src);
}

/**
 * Destroy window we created in constructor, if filter is inactive
 *
 * @param active
 */
void dsa::ImageOutputFilter::setActive(const bool active) {
	Filter::setActive(active);
	if(!active) {
		cv::destroyWindow(windowName_);
	}
}

/*
 * ImageReader.cpp
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#include <sensordata/ImageReader.h>

dsa::ImageReader::ImageReader(const std::string &filename, const int flags) {
	flags_ = flags;
	filename_ = filename;
	image_ = cv::imread(filename_, flags_);
}

dsa::ImageReader::~ImageReader() {
}

bool dsa::ImageReader::open(const std::string &filename, const int flags, bool force) {
	flags_= flags;

	if(filename.length() == 0)	{	// no filename given, assuming it was set previously
		close();
		if(filename_.length() > 0)	// open filename_ if any
			image_ = cv::imread(filename_, flags);
		else
			return(false);
	} else if(force || filename_.compare(filename) != 0 || flags_ != flags || image_.data == NULL) {	// filename given and we actually want to (force) read a new image
		filename_ = filename;
		image_ = cv::imread(filename_, flags);
	}
	return(image_.data != NULL);
}

bool dsa::ImageReader::openAsIs(const std::string &filename, bool force) {
	return(open(filename, -1, force));
}

bool dsa::ImageReader::openGrayscale(const std::string &filename, bool force) {
	return(open(filename, 0, force));
}

bool dsa::ImageReader::openRGB(const std::string &filename, bool force) {
	return(open(filename, 1, force));
}

bool dsa::ImageReader::isOpened() const {
	return(image_.data != NULL);	// successful = there is image data
}

void dsa::ImageReader::close() {
	if(image_.data != NULL)	// release image if any
		image_.release();
}

cv::Mat dsa::ImageReader::getImage() {
	if(image_.data == NULL)	// try to open again in case we failed previously
		open(filename_, flags_);
	return(image_);
}

std::string dsa::ImageReader::getFilename() const {
	return(filename_);
}

void dsa::ImageReader::setFilename(const std::string &filename) {
	if(filename_.compare(filename) != 0)	// check first if filenames are different
		close();
	filename_ = filename;
}

int dsa::ImageReader::getFlags() const {
	return(flags_);
}

void dsa::ImageReader::setFlags(const int flags) {
	if(flags_ != flags)	// close image in case we change the flags
		close();
	flags_ = flags;
}

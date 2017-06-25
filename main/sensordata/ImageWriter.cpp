/*
 * ImageWriter.cpp
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#include "ImageWriter.h"

dsa::ImageWriter::ImageWriter(const std::string &filename) {
	init();
	filename_ = filename;
}

dsa::ImageWriter::ImageWriter(cv::Mat &image, const std::string &filename) {
	init();
	image_ = image;
	filename_ = filename;
}

dsa::ImageWriter::~ImageWriter() {
}

void dsa::ImageWriter::init() {
	jpegQuality_ = 95;
	pngCompLevel_ = 9; // was originally 3, but 9 is way better as a default
	pxmIsBinary_ = 1;
}

std::vector<int> dsa::ImageWriter::getParams() {
	std::vector<int> params;
	if(filename_.length() > 4) {
		std::string suffix = filename_.substr(filename_.length()-4, 3);
		for(size_t i = 0; i < 3; i++) // lower-case letters are easier to compare
			suffix.at(i) = (char)tolower(suffix.at(i));
		if(suffix.compare("jpg") == 0) {
			params.push_back(CV_IMWRITE_JPEG_QUALITY);
			params.push_back(jpegQuality_);
		} else if(suffix.compare("png") == 0) {
			params.push_back(CV_IMWRITE_PNG_COMPRESSION);
			params.push_back(pngCompLevel_);
		} else if(suffix.compare("ppm") == 0 || suffix.compare("pgm") == 0 || suffix.compare("pbm") == 0) {
			params.push_back(CV_IMWRITE_PXM_BINARY);
			params.push_back((int)pxmIsBinary_);
		}
	}
	return(params);
}

bool dsa::ImageWriter::write() {
	return(imwrite(filename_, image_, getParams()));
}

bool dsa::ImageWriter::write(const std::string filename) {
	filename_ = filename;
	return((image_.data != NULL && filename_.length() > 4) ? imwrite(filename_, image_, getParams()) : false);
}

bool dsa::ImageWriter::write(cv::Mat &image, const std::string &filename) {
	image_ = image;
	if(filename.length() > 4)
		filename_ = filename;
	return((image_.data != NULL && filename_.length() > 4) ? imwrite(filename_, image_, getParams()) : false);
}

std::string dsa::ImageWriter::getFilename() {
	return(filename_);
}

void dsa::ImageWriter::setFilename(const std::string &filename) {
	filename_ = filename;
}

cv::Mat dsa::ImageWriter::getImage() {
	return(image_);
}

void dsa::ImageWriter::setImage(cv::Mat &image) {
	image_ = image;
}

// JPEG (lossy) compression quality
int dsa::ImageWriter::getJpegQuality() const {
	return(jpegQuality_);
}

void dsa::ImageWriter::setJpegQuality(const int percent) {
	jpegQuality_ = percent;
}

// PNG (lossless) compression level
int dsa::ImageWriter::getPngCompression() const {
	return(pngCompLevel_);
}

void dsa::ImageWriter::setPngCompression(const int level) {
	pngCompLevel_ = level;
}

// PXM binary setting (see PPM, PGM, PBM image compression codec)
bool dsa::ImageWriter::getPxmBinary() const {
	return(pxmIsBinary_);
}

void dsa::ImageWriter::setPxmBinary(bool isBinary) {
	pxmIsBinary_ = isBinary;
}

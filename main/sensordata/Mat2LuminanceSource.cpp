/*
 * Mat2LuminanceSource.cpp
 *
 *  Created on: 24.07.2013
 *      Author: jung
 */

#include "Mat2LuminanceSource.h"

dsa::Mat2LuminanceSource::Mat2LuminanceSource(cv::Mat &image) : GreyscaleLuminanceSource(mat2ArrayRef(image), image.cols, image.rows, 0, 0, image.cols, image.rows) {
}

dsa::Mat2LuminanceSource::~Mat2LuminanceSource() {
}

zxing::ArrayRef<char> dsa::Mat2LuminanceSource::mat2ArrayRef(cv::Mat &image) {
	size_t npixels = image.rows* image.cols;
	zxing::ArrayRef<char> matrix = zxing::ArrayRef<char>(npixels);

	std::vector<char>& values = matrix->values();
	for(size_t i = 0; i < npixels; i++)
		values[i] = (char)(image.data[i] >> 1);	// seems like only [0, 128] values are valid in the target data structure
	// maybe a memcpy can do this faster, would be a dirty hack, though

	return(matrix);
}

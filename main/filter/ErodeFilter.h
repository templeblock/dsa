/*
 * ErodeFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#erode
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef ERODEFILTER_H_
#define ERODEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class ErodeFilter : public ImageFilter {

	cv::Mat element_;
	cv::Point anchor_;
	int iterations_;
	int borderType_;
	cv::Scalar borderValue_;

	void init();

public:

	cv::Mat* element;
	cv::Point* anchor;
	int* iterations;
	int* borderType;
	cv::Scalar* borderValue;

	ErodeFilter(cv::Mat& element, cv::Point anchor=cv::Point(-1, -1), int iterations=1, int borderType=cv::BORDER_CONSTANT, const cv::Scalar& borderValue=cv::morphologyDefaultBorderValue());
	virtual ~ErodeFilter();

	void apply();
};

}
#endif /* ERODEFILTER_H_ */

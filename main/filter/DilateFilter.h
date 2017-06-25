/*
 * DilateFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#dilate
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef DILATEFILTER_H_
#define DILATEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class DilateFilter : public ImageFilter {

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

	DilateFilter(cv::Mat element, cv::Point anchor=cv::Point(-1, -1), int iterations=1, int borderType=cv::BORDER_CONSTANT, const cv::Scalar& borderValue=cv::morphologyDefaultBorderValue());
	virtual ~DilateFilter();

	void apply();
};

}
#endif /* DILATEFILTER_H_ */

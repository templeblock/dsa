/*
 * MorphologyExFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#morphologyex
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef MORPHOLOGYEXFILTER_H_
#define MORPHOLOGYEXFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class MorphologyExFilter : public ImageFilter {

	int op_;
	cv::Mat element_;
	cv::Point anchor_;
	int iterations_;
	int borderType_;
	cv::Scalar borderValue_;

	void init();

public:

	int* op;
	cv::Mat* element;
	cv::Point* anchor;
	int* iterations;
	int* borderType;
	cv::Scalar* borderValue;

	MorphologyExFilter(int op, cv::Mat& element, cv::Point anchor=cv::Point(-1, -1), int iterations=1, int borderType=cv::BORDER_CONSTANT, const cv::Scalar& borderValue=cv::morphologyDefaultBorderValue());
	virtual ~MorphologyExFilter();

	void apply();
};

}
#endif /* MORPHOLOGYEXFILTER_H_ */

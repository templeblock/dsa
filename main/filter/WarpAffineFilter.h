/*
 * WarpAffineFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#warpaffine
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef WARPAFFINEFILTER_H_
#define WARPAFFINEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class WarpAffineFilter : public ImageFilter {

	cv::Mat M_;
	cv::Size dsize_;
	int flags_;
	int borderMode_;
	cv::Scalar borderValue_;

	void init();

public:

	cv::Mat* M;
	cv::Size* dsize;
	int* flags;
	int* borderMode;
	cv::Scalar* borderValue;

	WarpAffineFilter(cv::Mat &M, cv::Size dsize, int flags=cv::INTER_LINEAR, int borderMode=cv::BORDER_CONSTANT, const cv::Scalar& borderValue=cv::Scalar());
	virtual ~WarpAffineFilter();

	void apply();
};

}
#endif /* WARPAFFINEFILTER_H_ */

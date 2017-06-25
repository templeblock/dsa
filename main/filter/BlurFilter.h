/*
 * BlurFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#blur
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef BLURFILTER_H_
#define BLURFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class BlurFilter : public ImageFilter {

	cv::Size ksize_;
	cv::Point anchor_;
	int borderType_;

	void init();

public:

	cv::Size* ksize;
	cv::Point* anchor;
	int* borderType;

	BlurFilter(cv::Size ksize, cv::Point anchor=cv::Point(-1,-1), int borderType=cv::BORDER_DEFAULT);
	virtual ~BlurFilter();

	void apply();
};

}
#endif /* BLURFILTER_H_ */

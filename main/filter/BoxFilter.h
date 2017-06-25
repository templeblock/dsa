/*
 * BoxFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#boxfilter
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef BOXFILTER_H_
#define BOXFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class BoxFilter : public ImageFilter {

	cv::Size ksize_;
	cv::Point anchor_;
	int normalize_;
	int borderType_;
	int ddepth_;

	void init();

public:

	cv::Size* ksize;
	cv::Point* anchor;
	int* normalize;
	int* borderType;
	int* ddepth;

	BoxFilter(int ddepth, cv::Size ksize, cv::Point anchor=cv::Point(-1,-1), bool normalize=true, int borderType=cv::BORDER_DEFAULT);
	virtual ~BoxFilter();

	void apply();
};

}
#endif /* BOXFILTER_H_ */

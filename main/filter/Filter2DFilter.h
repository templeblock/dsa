/*
 * Filter2DFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#filter2d
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef FILTER2DFILTER_H_
#define FILTER2DFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class Filter2DFilter : public ImageFilter {

	int ddepth_;
	cv::Mat kernel_;
	cv::Point anchor_;
	double delta_;
	int borderType_;

	void init();

public:

	int* ddepth;
	cv::Mat* kernel;
	cv::Point* anchor;
	double* delta;
	int* borderType;

	Filter2DFilter(int ddepth, cv::Mat& kernel, cv::Point anchor=cv::Point(-1, -1), double delta=0, int borderType=cv::BORDER_DEFAULT);
	virtual ~Filter2DFilter();

	void apply();
};

}
#endif /* FILTER2DFILTER_H_ */

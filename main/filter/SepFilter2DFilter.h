/*
 * SepFilter2DFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#sepfilter2d
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef SEPFILTER2DFILTER_H_
#define SEPFILTER2DFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class SepFilter2DFilter : public ImageFilter {

	int ddepth_;
	cv::Mat rowKernel_;
	cv::Mat columnKernel_;
	cv::Point anchor_;
	double delta_;
	int borderType_;

	void init();

public:

	int* ddepth;
	cv::Mat* rowKernel;
	cv::Mat* columnKernel;
	cv::Point* anchor;
	double* delta;
	int* borderType;

	SepFilter2DFilter(int ddepth, cv::Mat& rowKernel, cv::Mat& columnKernel, cv::Point anchor=cv::Point(-1, -1), double delta=0, int borderType=cv::BORDER_DEFAULT);
	virtual ~SepFilter2DFilter();

	void apply();
};

}
#endif /* SEPFILTER2DFILTER_H_ */

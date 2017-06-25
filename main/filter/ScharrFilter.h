/*
 * ScharrFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#scharr
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef SCHARRFILTER_H_
#define SCHARRFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class ScharrFilter : public ImageFilter {

	int ddepth_;
	int xorder_;
	int yorder_;
	double scale_;
	double delta_;
	int borderType_;

	void init();

public:

	int* ddepth;
	int* xorder;
	int* yorder;
	double* scale;
	double* delta;
	int* borderType;

	ScharrFilter(int ddepth, int xorder, int yorder, double scale=1, double delta=0, int borderType=cv::BORDER_DEFAULT);
	virtual ~ScharrFilter();

	void apply();
};

}
#endif /* SCHARRFILTER_H_ */

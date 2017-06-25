/*
 * LaplacianFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#laplacian
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef LAPLACIANFILTER_H_
#define LAPLACIANFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class LaplacianFilter : public ImageFilter {

	int ddepth_;
	int ksize_;
	double scale_;
	double delta_;
	int borderType_;

	void init();

public:

	int* ddepth;
	int* ksize;
	double* scale;
	double* delta;
	int* borderType;

	LaplacianFilter(int ddepth, int ksize=1, double scale=1, double delta=0, int borderType=cv::BORDER_DEFAULT);
	virtual ~LaplacianFilter();

	void apply();
};

}
#endif /* LAPLACIANFILTER_H_ */

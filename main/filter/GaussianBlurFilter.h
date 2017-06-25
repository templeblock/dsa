/*
 * GaussianBlurFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#gaussianblur
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef GAUSSIANBLURFILTER_H_
#define GAUSSIANBLURFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class GaussianBlurFilter : public ImageFilter {

	cv::Size ksize_;
	double sigmaX_;
	double sigmaY_;
	int borderType_;

	void init();

public:

	cv::Size* ksize;
	double* sigmaX;
	double* sigmaY;
	int* borderType;

	GaussianBlurFilter(cv::Size ksize, double sigmaX, double sigmaY=0, int borderType=cv::BORDER_DEFAULT);
	virtual ~GaussianBlurFilter();

	void apply();
};

}
#endif /* GAUSSIANBLURFILTER_H_ */

/*
 * BilateralFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#bilateralfilter
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef BILATERALFILTER_H_
#define BILATERALFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class BilateralFilter : public ImageFilter {

	int d_;
	double sigmaColor_;
	double sigmaSpace_;
	int borderType_;

	void init();

public:

	int* d;
	double* sigmaColor;
	double* sigmaSpace;
	int* borderType;

	BilateralFilter(int d, double sigmaColor, double sigmaSpace, int borderType=cv::BORDER_DEFAULT);
	virtual ~BilateralFilter();

	void apply();
};

}
#endif /* BILATERALFILTER_H_ */

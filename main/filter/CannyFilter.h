/*
 * CannyFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#canny
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef CANNYFILTER_H_
#define CANNYFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class CannyFilter : public ImageFilter {

	double threshold1_;
	double threshold2_;
	int apertureSize_;
	bool L2gradient_;

	void init();

public:

	double* threshold1;
	double* threshold2;
	int* apertureSize;
	bool* L2gradient;

	CannyFilter(double threshold1, double threshold2, int apertureSize=3, bool L2gradient=false);
	virtual ~CannyFilter();

	void apply();
};

}
#endif /* CANNYFILTER_H_ */

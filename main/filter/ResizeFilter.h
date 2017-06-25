/*
 * ResizeFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#resize
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef RESIZEFILTER_H_
#define RESIZEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class ResizeFilter : public ImageFilter {

	cv::Size dsize_;
	double fx_;
	double fy_;
	int interpolation_;

	void init();

public:

	// remember: this makes sense! We use public pointers to private data for a reason, see the documentation for details.
	cv::Size* dsize;
	double* fx;
	double* fy;
	int* interpolation;

	ResizeFilter(cv::Size dsize, int interpolation=cv::INTER_LINEAR);
	ResizeFilter(double fxy, int interpolation=cv::INTER_LINEAR);
	ResizeFilter(double fx, double fy, int interpolation=cv::INTER_LINEAR);

	virtual ~ResizeFilter();

	void apply();
};

}
#endif /* RESIZEFILTER_H_ */

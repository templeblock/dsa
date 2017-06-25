/*
 * RotationFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#warpaffine
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef ROTATIONFILTER_H_
#define ROTATIONFILTER_H_

#include "ImageFilter.h"
#include <generic/helper.h>

namespace dsa {

class RotationFilter : public ImageFilter {

	double angle_;
	cv::Size dsize_;

	void init();
	bool isReady();

public:

	double *angle;
	cv::Size *dsize;

	RotationFilter(double angle, cv::Size dsize);
	virtual ~RotationFilter();

	void apply();
};

}
#endif /* ROTATIONFILTER_H_ */

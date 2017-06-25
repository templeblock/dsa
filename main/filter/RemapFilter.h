/*
 * RemapFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#remap
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef REMAPFILTER_H_
#define REMAPFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class RemapFilter : public ImageFilter {

	cv::Mat map1_;
	cv::Mat map2_;
	int interpolation_;
	int borderMode_;
	cv::Scalar borderValue_;

	void init();

public:

	cv::Mat* map1;
	cv::Mat* map2;
	int* interpolation;
	int* borderMode;
	cv::Scalar* borderValue;

	RemapFilter(cv::Mat& map1, cv::Mat& map2, int interpolation, int borderMode=cv::BORDER_CONSTANT, const cv::Scalar& borderValue=cv::Scalar());
	virtual ~RemapFilter();

	void apply();
};

}
#endif /* REMAPFILTER_H_ */

/*
 * MaskFilter.h
 *
 * 	Applies a mask to an image, i.e. filters every pixel from the image where the corresponding pixel in the mask is 0.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef MASKFILTER_H_
#define MASKFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class MaskFilter : public ImageFilter {

	bool transparency_;
	int alpha_;
	cv::Scalar bgcolor_;
	cv::Mat mask_;

	void init();

public:

	int* alpha;
	cv::Scalar* bgcolor;
	cv::Mat* mask;

	MaskFilter(cv::Mat &mask, cv::Scalar color=cv::Scalar(255));
	MaskFilter(cv::Mat &mask, const int alpha);
	virtual ~MaskFilter();

	void apply();

	void setMask(cv::Mat &mask);
};

}
#endif /* MASKFILTER_H_ */

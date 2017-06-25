/*
 * CopyMakeBorderFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#copymakeborder
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef COPYMAKEBORDERFILTER_H_
#define COPYMAKEBORDERFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class CopyMakeBorderFilter: public ImageFilter {

	int top_;
	int bottom_;
	int left_;
	int right_;
	int borderType_;
	cv::Scalar value_;

	void init();

public:

	int* top;
	int* bottom;
	int* left;
	int* right;
	int* borderType;
	cv::Scalar* value;

	CopyMakeBorderFilter(int top, int bottom, int left, int right, int borderType, const cv::Scalar& value=cv::Scalar());
	virtual ~CopyMakeBorderFilter();

	void apply();
};

}
#endif /* COPYMAKEBORDERFILTER_H_ */

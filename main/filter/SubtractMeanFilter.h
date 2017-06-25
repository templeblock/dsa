/*
 * SubtractMeanFilter.cpp
 *
 * Subtract the mean value from the image values
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef SUBTRACTMEANFILTER_H_
#define SUBTRACTMEANFILTER_H_

#include <generic/types.h>
#include "ImageFilter.h"

namespace dsa {

class SubtractMeanFilter : public ImageFilter {

	cv::Mat mask_;

	void init();

public:

	cv::Mat* mask;

	SubtractMeanFilter(const cv::Mat& mask=cv::Mat());
	virtual ~SubtractMeanFilter();

	void apply();
};

}
#endif /* SUBTRACTMEANFILTER_H_ */

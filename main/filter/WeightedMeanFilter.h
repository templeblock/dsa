/*
 * WeightedMeanFilter.cpp
 *
 * 	See
 *
 *	2D Filter that assigns the mean value of the kernel to the pixel in the middle of the kernel
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef WEIGHTEDMEANFILTER_H_
#define WEIGHTEDMEANFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class WeightedMeanFilter : public ImageFilter {

	cv::Mat kernel_;

	void init();

public:

	WeightedMeanFilter(int ksize=3, float centerWeight=1.0f/9.0f);
	virtual ~WeightedMeanFilter();

	void apply();
};

}
#endif /* WEIGHTEDMEANFILTER_H_ */

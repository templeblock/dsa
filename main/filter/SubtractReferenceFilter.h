/*
 * SubtractReferenceFilter.cpp
 *
 * Subtract a reference image from another image of the same size
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef SUBTRACTREFERENCEFILTER_H_
#define SUBTRACTREFERENCEFILTER_H_

#include <generic/types.h>
#include "ImageFilter.h"

namespace dsa {

class SubtractReferenceFilter : public ImageFilter {

	cv::Mat reference_;
	cv::Mat mask_;
	cv::Scalar referenceMean_;

	void init();

public:

	cv::Mat* reference;
	cv::Mat* mask;

	SubtractReferenceFilter(const cv::Mat& reference, const cv::Mat& mask=cv::Mat());
	virtual ~SubtractReferenceFilter();

	void apply();

	void setReference(const cv::Mat& reference, const cv::Mat& mask=cv::Mat());
};

}
#endif /* SUBTRACTREFERENCEFILTER_H_ */

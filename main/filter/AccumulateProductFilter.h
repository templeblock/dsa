/*
 * AccumulateProductFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/motion_analysis_and_object_tracking.html#accumulateproduct
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef ACCUMULATEPRODUCTFILTER_H_
#define ACCUMULATEPRODUCTFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class AccumulateProductFilter : public ImageFilter {

	cv::Mat mask_;
	cv::Mat src2_;

	void init();

public:

	cv::Mat* mask;
	cv::Mat* src2;

	AccumulateProductFilter(const cv::Mat& src2, const cv::Mat& mask=cv::Mat());
	virtual ~AccumulateProductFilter();

	void apply();
};

}
#endif /* ACCUMULATEPRODUCTFILTER_H_ */

/*
 * AccumulateSquareFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/motion_analysis_and_object_tracking.html#accumulatesquare
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef ACCUMULATESQUAREFILTER_H_
#define ACCUMULATESQUAREFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class AccumulateSquareFilter : public ImageFilter {

	cv::Mat mask_;

	void init();

public:

	cv::Mat* mask;

	AccumulateSquareFilter(const cv::Mat& mask=cv::Mat());
	virtual ~AccumulateSquareFilter();

	void apply();
};

}
#endif /* ACCUMULATESQUAREFILTER_H_ */

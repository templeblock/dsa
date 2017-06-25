/*
 * AccumulateFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/motion_analysis_and_object_tracking.html#accumulate
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef ACCUMULATEFILTER_H_
#define ACCUMULATEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class AccumulateFilter : public ImageFilter {

	cv::Mat mask_;

	void init();

public:

	cv::Mat* mask;

	AccumulateFilter(const cv::Mat& mask=cv::Mat());
	virtual ~AccumulateFilter();

	void apply();
};

}
#endif /* ACCUMULATEFILTER_H_ */

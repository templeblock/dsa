/*
 * AccumulateWeightedFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/motion_analysis_and_object_tracking.html#accumulateweighted
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef ACCUMULATEWEIGHTEDFILTER_H_
#define ACCUMULATEWEIGHTEDFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class AccumulateWeightedFilter : public ImageFilter {

	double alpha_;
	cv::Mat mask_;

	void init ();

public:

	double* alpha;
	cv::Mat* mask;

	AccumulateWeightedFilter(double alpha, const cv::Mat& mask=cv::Mat());
	virtual ~AccumulateWeightedFilter();

	void apply ();
};

}
#endif /* ACCUMULATEWEIGHTEDFILTER_H_ */

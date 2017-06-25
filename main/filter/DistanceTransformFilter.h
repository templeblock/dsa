/*
 * DistanceTransformFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform
 *
 *	for details on the image filter.
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#ifndef DISTANCETRANSFORMFILTER_H_
#define DISTANCETRANSFORMFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class DistanceTransformFilter : public EvaluationFilter {

	cv::Mat labels_;
	int distanceType_;
	int maskSize_;

	void init();
	void evaluate();

public:

	cv::Mat* labels;
	int* distanceType;
	int* maskSize;

	DistanceTransformFilter(cv::Mat& labels, int distanceType, int maskSize);
	virtual ~DistanceTransformFilter();

};

}
#endif /* DISTANCETRANSFORMFILTER_H_ */

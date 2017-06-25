/*
 * HoughCirclesFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghcircles
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef HOUGHCIRCLESFILTER_H_
#define HOUGHCIRCLESFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class HoughCirclesFilter : public EvaluationFilter {

	cv::Mat circles_;
	int method_;
	double dp_;
	double minDist_;
	double param1_;
	double param2_;
	int minRadius_;
	int maxRadius_;

	void init();
	void evaluate();

public:

	cv::Mat* circles;
	int* method;
	double* dp;
	double* minDist;
	double* param1;
	double* param2;
	int* minRadius;
	int* maxRadius;

	HoughCirclesFilter(cv::Mat& circles, int method, double dp, double minDist, double param1=100, double param2=100, int minRadius=0, int maxRadius=0);
	virtual ~HoughCirclesFilter();

};

}
#endif /* HOUGHCIRCLESFILTER_H_ */

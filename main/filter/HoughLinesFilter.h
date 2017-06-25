/*
 * HoughLinesFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlines
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef HOUGHLINESFILTER_H_
#define HOUGHLINESFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class HoughLinesFilter : public EvaluationFilter {

	cv::Mat lines_;
	double rho_;
	double theta_;
	int threshold_;
	double srn_;
	double stn_;

	void init();
	void evaluate();

public:

	cv::Mat* lines;
	double* rho;
	double* theta;
	int* threshold;
	double* srn;
	double* stn;

	HoughLinesFilter(cv::Mat& lines, double rho, double theta, int threshold, double srn=0, double stn=0);
	virtual ~HoughLinesFilter();

};

}
#endif /* HOUGHLINESFILTER_H_ */

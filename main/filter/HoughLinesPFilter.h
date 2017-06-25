/*
 * HoughLinesPFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlinesp
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef HOUGHLINESPFILTER_H_
#define HOUGHLINESPFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class HoughLinesPFilter : public EvaluationFilter {

	cv::Mat lines_;
	double rho_;
	double theta_;
	int threshold_;
	double minLineLength_;
	double maxLineGap_;

	void init();
	void evaluate();

public:

	cv::Mat* lines;
	double* rho;
	double* theta;
	int* threshold;
	double* minLineLength;
	double* maxLineGap;

	HoughLinesPFilter(cv::Mat& lines, double rho, double theta, int threshold, double minLineLength=0, double maxLineGap=0);
	virtual ~HoughLinesPFilter();

};

}
#endif /* HOUGHLINESPFILTER_H_ */

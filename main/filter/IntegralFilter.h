/*
 * IntegralFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#integral
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef INTEGRALFILTER_H_
#define INTEGRALFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class IntegralFilter : public EvaluationFilter {

	enum Mode { S, SS, SST };	// represents: only sum, sum and squaresum, sum, squaresum and tilted

	cv::Mat sum_;
	cv::Mat sqsum_;
	cv::Mat tilted_;
	int sdepth_;

	Mode mode_;

	void init();
	void evaluate();

public:

	cv::Mat* sum;
	cv::Mat* sqsum;
	cv::Mat* tilted;
	int* sdepth;

	IntegralFilter(cv::Mat& sum, int sdepth=-1);
	IntegralFilter(cv::Mat& sum, cv::Mat& sqsum, int sdepth=-1);
	IntegralFilter(cv::Mat& sum, cv::Mat& sqsum, cv::Mat& tilted, int sdepth=-1);
	virtual ~IntegralFilter();

};

}
#endif /* INTEGRALFILTER_H_ */

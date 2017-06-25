/*
 * GrabCutFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#grabcut
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef GRABCUTFILTER_H_
#define GRABCUTFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class GrabCutFilter : public EvaluationFilter {

	cv::Mat mask_;
	cv::Rect rect_;
	cv::Mat bgdModel_;
	cv::Mat fgdModel_;
	int iterCount_;
	int mode_;

	void init();
	void evaluate();

public:

	cv::Mat* mask;
	cv::Rect* rect;
	cv::Mat* bgdModel;
	cv::Mat* fgdModel;
	int* iterCount;
	int* mode;

	GrabCutFilter(cv::Mat& mask, cv::Rect& rect, cv::Mat& bgdModel, cv::Mat& fgdModel, int iterCount, int mode=cv::GC_EVAL);
	virtual ~GrabCutFilter();

};

}
#endif /* GRABCUTFILTER_H_ */

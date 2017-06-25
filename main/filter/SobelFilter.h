/*
 * SobelFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#sobel
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef SOBELFILTER_H_
#define SOBELFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class SobelFilter : public EvaluationFilter {

	int ddepth_;
	int xorder_;
	int yorder_;
	int ksize_;
	double scale_;
	double delta_;
	int borderType_;

	void init();
	void evaluate();

public:

	int* ddepth;
	int* xorder;
	int* yorder;
	int* ksize;
	double* scale;
	double* delta;
	int* borderType;

	SobelFilter(int ddepth, int xorder, int yorder, int ksize=3, double scale=1, double delta=0, int borderType=cv::BORDER_DEFAULT);
	virtual ~SobelFilter();

};

}
#endif /* SOBELFILTER_H_ */

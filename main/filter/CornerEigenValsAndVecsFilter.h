/*
 * CornerEigenValsAndVecsFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#cornereigenvalsandvecs
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef CORNEREIGENVALSANDVECSFILTER_H_
#define CORNEREIGENVALSANDVECSFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class CornerEigenValsAndVecsFilter : public EvaluationFilter {

	int blockSize_;
	int apertureSize_;
	int borderType_;

	void init();
	void evaluate();

public:

	int* blockSize;
	int* apertureSize;
	int* borderType;

	CornerEigenValsAndVecsFilter(int blockSize, int apertureSize, int borderType=cv::BORDER_DEFAULT);
	virtual ~CornerEigenValsAndVecsFilter();

};

}
#endif /* CORNEREIGENVALSANDVECSFILTER_H_ */

/*
 * CornerMinEigenValFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#cornermineigenval
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef CORNERMINEIGENVALFILTER_H_
#define CORNERMINEIGENVALFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class CornerMinEigenValFilter : public EvaluationFilter {

	int blockSize_;
	int apertureSize_;
	int borderType_;

	void init();
	void evaluate();

public:

	int* blockSize;
	int* apertureSize;
	int* borderType;

	CornerMinEigenValFilter(int blockSize, int apertureSize=3, int borderType=cv::BORDER_DEFAULT);
	virtual ~CornerMinEigenValFilter();

};

}
#endif /* CORNERMINEIGENVALFILTER_H_ */

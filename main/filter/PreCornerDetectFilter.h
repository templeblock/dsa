/*
 * PreCornerDetectFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#precornerdetect
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef PRECORNERDETECTFILTER_H_
#define PRECORNERDETECTFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class PreCornerDetectFilter : public EvaluationFilter {

	int apertureSize_;
	int borderType_;

	void init();
	void evaluate();

public:

	int* apertureSize;
	int* borderType;

	PreCornerDetectFilter(int apertureSize, int borderType=cv::BORDER_DEFAULT);
	virtual ~PreCornerDetectFilter();

};

}
#endif /* PRECORNERDETECTFILTER_H_ */

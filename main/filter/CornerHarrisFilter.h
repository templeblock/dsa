/*
 * CornerHarrisFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#cornerharris
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef CORNERHARRISFILTER_H_
#define CORNERHARRISFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class CornerHarrisFilter : public EvaluationFilter {

	int blockSize_;
	int apertureSize_;
	double k_;
	int borderType_;

	void init();
	void evaluate();

public:

	int* blockSize;
	int* apertureSize;
	double* k;
	int* borderType;

	CornerHarrisFilter(int blockSize, int apertureSize, double k, int borderType=cv::BORDER_DEFAULT);
	virtual ~CornerHarrisFilter();

};

}
#endif /* CORNERHARRISFILTER_H_ */

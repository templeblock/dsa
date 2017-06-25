/*
 * ThresholdFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#threshold
 *
 *	for details on the image filter.
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#ifndef THRESHOLDFILTER_H_
#define THRESHOLDFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class ThresholdFilter: public ImageFilter {

	double thresh_;
	double maxVal_;
	int thresholdType_;

	void init();

public:

	double* thresh;
	double* maxVal;
	int* thresholdType;

	ThresholdFilter(double thresh, double maxVal, int thresholdType);
	virtual ~ThresholdFilter();

	void apply();
};

}
#endif /* THRESHOLDFILTER_H_ */

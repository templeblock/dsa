/*
 * AdaptiveThresholdFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#adaptivethreshold
 *
 *	for details on the image filter.
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#ifndef ADAPTIVETHRESHOLDFILTER_H_
#define ADAPTIVETHRESHOLDFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class AdaptiveThresholdFilter : public ImageFilter {

	double maxValue_;
	int adaptiveMethod_;
	int thresholdType_;
	int blockSize_;
	double C_;

	void init();

public:

	double* maxValue;
	int* adaptiveMethod;
	int* thresholdType;
	int* blockSize;
	double* C;

	AdaptiveThresholdFilter(double maxValue, int adaptiveMethod, int thresholdType, int blockSize, double C);
	virtual ~AdaptiveThresholdFilter();

	void apply();
};

}
#endif /* ADAPTIVETHRESHOLDFILTER_H_ */

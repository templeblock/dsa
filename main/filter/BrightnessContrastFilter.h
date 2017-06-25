/*
 * BrightnessContrastFilter.cpp
 *
 *  Uses convertTo method to adjust brightness and contrast.
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/core/doc/basic_structures.html#mat-convertto
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef BRIGHTNESSCONTRASTFILTER_H_
#define BRIGHTNESSCONTRASTFILTER_H_

#include <generic/types.h>
#include "ImageFilter.h"

namespace dsa {

class BrightnessContrastFilter : public ImageFilter {

	double alpha_; 	// controls brightness
	double beta_;	// controls contrast

	void init();

public:

	double* alpha;
	double* beta;

	BrightnessContrastFilter(double alpha, double beta);
	virtual ~BrightnessContrastFilter();

	void apply();
};

}
#endif /* BRIGHTNESSCONTRASTFILTER_H_ */

/*
 * UnsharpMaskFilter.h
 *
 * 	See
 *
 *
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef UNSHARPMASKFILTER_H_
#define UNSHARPMASKFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class UnsharpMaskFilter : public ImageFilter {
	int ksize_;	// parameter of the gaussian filter
	double weight_;	// amount of weight of the blurred image

	void init();

public:

	int* ksize;
	double* weight;

	UnsharpMaskFilter(int ksize=3, double weight=0.5);
	virtual ~UnsharpMaskFilter();

	void apply();
};

}
#endif /* UNSHARPMASKFILTER_H_ */

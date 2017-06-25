/*
 * EmptyMaskFilter.cpp
 *
 * Create an empty mask (black image) of the same size as input image
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef EMPTYMASKFILTER_H_
#define EMPTYMASKFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class EmptyMaskFilter : public ImageFilter {

	void init();

public:

	EmptyMaskFilter();
	virtual ~EmptyMaskFilter();

	void apply();
};

}
#endif /* EMPTYMASKFILTER_H_ */

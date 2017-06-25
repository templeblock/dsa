/*
 * FlipFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#void%20flip%28InputArray%20src,%20OutputArray%20dst,%20int%20flipCode%29
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef FLIPFILTER_H_
#define FLIPFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class FlipFilter : public ImageFilter {

	int flipCode_;
	void init();

public:

	FlipFilter(int flipCode=1);
	virtual ~FlipFilter();

	void apply();
};

}
#endif /* FLIPFILTER_H_ */

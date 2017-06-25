/*
 * HasChangedFilter.h
 *
 *	Determine whether there was a significant change compared to the previous image. A significant change is a change in a number of pixels that represent a ratio of the image greater than a given threshold.
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef HASCHANGEDFILTER_H_
#define HASCHANGEDFILTER_H_

#include "ImageBufferFilter.h"

namespace dsa {

class HasChangedFilter : public ImageBufferFilter {

	bool hasChanged_;
	bool changeRatioThreshold_;
	void evaluate();

public:

	bool* hasChanged;

	HasChangedFilter(const double changeRatioThreshold=0.0);
	virtual ~HasChangedFilter();
};

}
#endif /* HASCHANGEDFILTER_H_ */

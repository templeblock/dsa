/*
 * CopyImageFilter.h
 *
 * Provides copies of its input images as output
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#ifndef COPYIMAGEFILTER_H_
#define COPYIMAGEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class CopyImageFilter : public ImageFilter {

	void init();

public:

	CopyImageFilter();
	virtual ~CopyImageFilter();

	void apply();
};

}
#endif /* COPYIMAGEFILTER_H_ */

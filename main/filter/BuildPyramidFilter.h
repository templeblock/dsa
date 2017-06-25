/*
 * buildPyramidFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#buildpyramid
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef BUILDPYRAMIDFILTER_H_
#define BUILDPYRAMIDFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class BuildPyramidFilter : public ImageFilter {

	int maxlevel_;

	void init();

public:

	int* maxlevel;
	BuildPyramidFilter(int maxlevel);
	virtual ~BuildPyramidFilter();

	void apply();
};

}
#endif /* BUILDPYRAMIDFILTER_H_ */

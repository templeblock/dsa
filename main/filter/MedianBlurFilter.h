/*
 * MedianBlurFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#medianblur
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef MEDIANBLURFILTER_H_
#define MEDIANBLURFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class MedianBlurFilter : public ImageFilter {

	int ksize_;

	void init();

public:

	int* ksize;

	MedianBlurFilter(int ksize=3);
	virtual ~MedianBlurFilter();

	void apply();
};

}
#endif /* MEDIANBLURFILTER_H_ */

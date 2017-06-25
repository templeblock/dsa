/*
 * PyrUpFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#pyrup
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef PYRUPFILTER_H_
#define PYRUPFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class PyrUpFilter : public ImageFilter {

	cv::Size dstsize_;

	void init();

public:

	cv::Size* dstsize;

	PyrUpFilter(const cv::Size& dstsize=cv::Size());
	virtual ~PyrUpFilter();

	void apply();
};

}
#endif /* PYRUPFILTER_H_ */

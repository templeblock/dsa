/*
 * PyrDownFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/filtering.html#pyrdown
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef PYRDOWNFILTER_H_
#define PYRDOWNFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class PyrDownFilter : public ImageFilter {

	cv::Size dstsize_;

	void init();

public:

	cv::Size* dstsize;

	PyrDownFilter(const cv::Size& dstsize=cv::Size());
	virtual ~PyrDownFilter();

	void apply();
};

}
#endif /* PYRDOWNFILTER_H_ */

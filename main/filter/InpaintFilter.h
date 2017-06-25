/*
 * InpaintFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/photo/doc/inpainting.html#inpaint
 *
 *	for details on the image filter.
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#ifndef INPAINTFILTER_H_
#define INPAINTFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class InpaintFilter: public ImageFilter {

	cv::Mat inpaintMask_;
	double inpaintRadius_;
	int flags_;

	void init();

public:

	cv::Mat* inpaintMask;
	double* inpaintRadius;
	int* flags;

	InpaintFilter(cv::Mat& inpaintMask, double inpaintRadius, int flags);
	virtual ~InpaintFilter();

	void apply();
};

}
#endif /* INPAINTFILTER_H_ */

/*
 * CvtColorFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#cvtcolor
 *
 *	for details on the image filter.
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#ifndef CVTCOLORFILTER_H_
#define CVTCOLORFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class CvtColorFilter : public ImageFilter {

	int code_;
	int dstCn_;

	void init();

public:

	int* code;
	int* dstCn;

	CvtColorFilter(int code, int dstCn=0);
	virtual ~CvtColorFilter();

	void apply();
};

}
#endif /* CVTCOLORFILTER_H_ */

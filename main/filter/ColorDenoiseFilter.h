/*
 * ColorDenoiseFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/trunk/modules/photo/doc/denoising.html#fastnlmeansdenoisingcolored
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef COLORDENOISEFILTER_H_
#define COLORDENOISEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class ColorDenoiseFilter : public dsa::ImageFilter {
	float h_;
	float hColor_;
	int templateWindowSize_;
	int searchWindowSize_;

	void init();

public:
	float* h;
	float* hColor;
	int* templateWindowSize;
	int* searchWindowSize;

	ColorDenoiseFilter(float h=3, float hColor=3, int templateWindowSize=7, int searchWindowSize=21);
	virtual ~ColorDenoiseFilter();

	void apply();
};

}
#endif /* COLORDENOISEFILTER_H_ */

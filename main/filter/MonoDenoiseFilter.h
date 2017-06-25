/*
 * MonoDenoiseFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/histograms.html#equalizehist
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef MONODENOISEFILTER_H_
#define MONODENOISEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class MonoDenoiseFilter : public ImageFilter {
	float h_;
	int templateWindowSize_;
	int searchWindowSize_;

	void init();

public:
	float* h;
	int* templateWindowSize;
	int* searchWindowSize;

	MonoDenoiseFilter(float h=3, int templateWindowSize=7, int searchWindowSize=21);
	virtual ~MonoDenoiseFilter();

	void apply();
};

}
#endif /* MONODENOISEFILTER_H_ */

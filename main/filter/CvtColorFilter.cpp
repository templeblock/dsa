/*
 * CvtColorFilter.cpp
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

#include "CvtColorFilter.h"

/**
 * Constructor
 *
 * @param code: Color space conversion code.
 * @param dstCn: Number of channels in the destination image. If the parameter is 0, the number of the channels is derived automatically from src and code.
 */
dsa::CvtColorFilter::CvtColorFilter(int code, int dstCn) {
	code_ = code;
	dstCn_ = dstCn;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::CvtColorFilter::~CvtColorFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::CvtColorFilter::init() {
	code = &code_;
	dstCn = &dstCn_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::CvtColorFilter::apply() {
	if(isReady()) {
		if(src->channels() == 1 && (code_ == (int)CV_RGB2GRAY || code_ == (int)CV_BGR2GRAY))
			dst = *src;	// no need for conversion if src has only one channel and target is gray
		else {
			cvtColor(*src, dst, *code, *dstCn);
		}
	}
}

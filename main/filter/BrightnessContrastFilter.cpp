/*
 * BrightnessContrastFilter.cpp
 *
 *  Uses convertTo method to adjust brightness and contrast.
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/core/doc/basic_structures.html#mat-convertto
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "BrightnessContrastFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::BrightnessContrastFilter::BrightnessContrastFilter(double alpha, double beta) {
	alpha_ = alpha;
	beta_ = beta;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BrightnessContrastFilter::~BrightnessContrastFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BrightnessContrastFilter::init() {
	alpha = &alpha_;
	beta = &beta_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::BrightnessContrastFilter::apply() {
	if(isReady()) {
		float a = (float)(*alpha);
		float b = (float)(*beta);
		UINT32 nPixels = src->rows * src->cols * src->channels();
		dst = cv::Mat(src->rows, src->cols, src->type());
		if(src->type() == CV_8UC1 || src->type() == CV_8UC3) {	// 8-bit gray
			// compute lookup table - this is really fast (only 256 floating point operations, the rest is just indirect addressing for each pixel)
			UINT8 LUT[256];
			for(size_t i = 0; i < 256; i++)
				LUT[i] = cv::saturate_cast<UINT8>(a*i + b);

			// apply values
			UINT8* srcPixels = src->data;
			UINT8* dstPixels = dst.data;
			for(size_t i = 0; i < nPixels; i++)
				dstPixels[i] = LUT[srcPixels[i]];
		} else // do it the conventional way
			src->convertTo(dst, -1, *alpha, *beta); // too slow for our 1Mbit 30fps application
	}
}

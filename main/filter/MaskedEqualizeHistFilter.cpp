/*
 * MaskedEqualizeHistFilter.h
 *
 * 	Ordinary histogram equalization performed on parts of an image defined by a mask
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/histograms.html#equalizehist
 *
 *	for details on the histogram equalization.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "MaskedEqualizeHistFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::MaskedEqualizeHistFilter::MaskedEqualizeHistFilter(cv::Mat &mask) {
	mask_ = mask;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::MaskedEqualizeHistFilter::~MaskedEqualizeHistFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::MaskedEqualizeHistFilter::init() {
	mask = &mask_;
}

/**
 * When filter is ready, perform histogram equalization on only those pixels p[i][j], where mask[i][j] > 0. Please note: the histogram of the entire image
 * will not be equalized as those parts where mask[i][j] == 0 are not considered at all. The implementation of the original opencv histogram equalization was
 * adapted in order to create this function. the mask is only allowed to have 0 or 1 for speed reasons
 */
void dsa::MaskedEqualizeHistFilter::apply() {
	if(isReady()) {
		const int hist_sz = 256;
		int hist[hist_sz] = {};
		size_t nPixels = src->rows * src->cols;
		// build histogram
		// handle masked and unmasked case separately => much faster
		if(mask->empty()) {
			for(size_t i = 0; i < nPixels; i++)
				hist[src->data[i]]++;
		} else {
			for(size_t i = 0; i < nPixels; i++)
				hist[src->data[i]] += mask->data[i]; // can be 1 or 0
		}

		// build lookup table (aka cumulative distribution function)
		float scale = 255.0f/(src->rows*src->cols);
		UINT8 lut[hist_sz+1];
		int sum = 0;
		for(int i = 0; i < hist_sz; i++) {
			sum += hist[i];
			lut[i] = cv::saturate_cast<UINT8>(round(sum*scale));
		}

		// set new values
		lut[0] = 0;
		//dst = src->clone();
		dst = cv::Mat(src->rows, src->cols, CV_8UC1);
		for(size_t i = 0; i < nPixels; i++) {
			UINT8 pixel = src->data[i];
			dst.data[i] = (mask->data[i] > 0) ? lut[pixel] : pixel;
		}
	}
}

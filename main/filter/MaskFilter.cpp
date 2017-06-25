/*
 * MaskFilter.h
 *
 * 	Applies a mask to an image, i.e. filters every pixel from the image where the corresponding pixel in the mask is 0.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "MaskFilter.h"

/**
 * Constructor
 *
 * @param mask: the mask to apply
 * @param color: the backgroundcolor for the rest of the image
 */
dsa::MaskFilter::MaskFilter(cv::Mat &mask, cv::Scalar color) {
	transparency_ = false;
	mask_ = mask;
	bgcolor_ = color;

	init();
}

/**
 * Constructor
 *
 * @param mask: the mask to apply
 * @param alpha: transparency for the rest in percent
 */
dsa::MaskFilter::MaskFilter(cv::Mat &mask, const int alpha) {
	transparency_ = true;
	alpha_ = alpha;
	mask_ = mask;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::MaskFilter::~MaskFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::MaskFilter::init() {
	alpha = &alpha_;
	mask = &mask_;
	bgcolor = &bgcolor_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::MaskFilter::apply() {
	if(isReady()) {
		dst = cv::Mat::ones(src->rows, src->cols, src->type())*(bgcolor->val[0]); // paint background image
		if(transparency_) {
			cv::Mat invmask_ = 1-(*mask);
			// copy stuff where mask = 1 and multiply by a factor 1.0+transparency/100.0
		} else
			src->copyTo(dst, *mask); // just copy values
	}
}

/**
 * Update the mask
 *
 * @param mask
 */
void dsa::MaskFilter::setMask(cv::Mat &mask) {
	mask_ = mask;
}

/*
 * InpaintFilter.cpp
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

#include "InpaintFilter.h"

/**
 * Constructor
 *
 * @param inpaintMask: Inpainting mask, 8-bit 1-channel image. Non-zero pixels indicate the area that needs to be inpainted.
 * @param inpaintRadius: Radius of a circular neighborhood of each point inpainted that is considered by the algorithm.
 * @param flags: Inpainting method that could be one of the following: INPAINT_NS Navier-Stokes based method, INPAINT_TELEA Method by Alexandru Telea
 */
dsa::InpaintFilter::InpaintFilter(cv::Mat &inpaintMask, double inpaintRadius, int flags) {
	inpaintMask_ = inpaintMask;
	inpaintRadius_ = inpaintRadius;
	flags_ = flags;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::InpaintFilter::~InpaintFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::InpaintFilter::init() {
	inpaintMask = &inpaintMask_;
	inpaintRadius = &inpaintRadius_;
	flags = &flags_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::InpaintFilter::apply() {
	if(isReady()) {
		cv::inpaint(*src, *inpaintMask, dst, *inpaintRadius, *flags);
	}
}

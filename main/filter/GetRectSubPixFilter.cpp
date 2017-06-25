/*
 * GetRectSubPixFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#getrectsubpix
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "GetRectSubPixFilter.h"

/**
 * Constructor
 *
 * @param patchSize: Size of the extracted patch.
 * @param center: Floating point coordinates of the center of the extracted rectangle within the source image. The center must be inside the image.
 * @param patchType: Depth of the extracted pixels. By default, they have the same depth as src.
 */
dsa::GetRectSubPixFilter::GetRectSubPixFilter(cv::Size patchSize, cv::Point2f center, int patchType) {
	patchSize_ = patchSize;
	center_ = center;
	patchType_ = patchType;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::GetRectSubPixFilter::~GetRectSubPixFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::GetRectSubPixFilter::init() {
	patchSize = &patchSize_;
	center = &center_;
	patchType = &patchType_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::GetRectSubPixFilter::apply() {
	if(isReady()) {
		cv::getRectSubPix(*src, *patchSize, *center, dst, *patchType);
	}
}

/*
 * BackProjectionFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/doc/tutorials/imgproc/histograms/back_projection/back_projection.html
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "BackprojectionFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::BackProjectionFilter::BackProjectionFilter() {
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BackProjectionFilter::~BackProjectionFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BackProjectionFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::BackProjectionFilter::apply() {
	if(isReady()) {
		//dst = *src;
		cv::Mat h, s, v;
		  h.create(src->size(), src->depth());
		  s.create(src->size(), src->depth());
		  v.create(src->size(), src->depth());
		  int hch[] = { 0, 0 };
		  int sch[] = { 1, 0 };
		  int vch[] = { 2, 0 };
		  mixChannels( src, 1, &h, 1, hch, 1);
		  mixChannels( src, 1, &s, 1, sch, 1);
		  mixChannels( src, 1, &v, 1, vch, 1);

		  imshow("Hue", h);
		  imshow("Saturation", s);
		  imshow("Value", v);
	}
}

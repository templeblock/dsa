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

#include "ColorDenoiseFilter.h"

/**
 * Constructor
 *
 * @param h: Parameter regulating filter strength. Big h value perfectly removes noise but also removes image details, smaller h value preserves details but also preserves some noise
 * @param hColor: The same as h but for color components. For most images value equals 10 will be enough to remove colored noise and do not distort colors
 * @param templateWindowSize: Size in pixels of the template patch that is used to compute weights. Should be odd. Recommended value 7 pixels
 * @param searchWindowSize: Size in pixels of the window that is used to compute weighted average for given pixel. Should be odd. Affect performance linearly: greater searchWindowsSize - greater denoising time. Recommended value 21 pixels
 */
dsa::ColorDenoiseFilter::ColorDenoiseFilter(float h, float hColor, int templateWindowSize, int searchWindowSize) {
	h_ = h;
	hColor_ = hColor;
	templateWindowSize_ = templateWindowSize;
	searchWindowSize_ = searchWindowSize;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ColorDenoiseFilter::~ColorDenoiseFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::ColorDenoiseFilter::init() {
	h = &h_;
	hColor = &hColor_;
	templateWindowSize = &templateWindowSize_;
	searchWindowSize = &searchWindowSize_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::ColorDenoiseFilter::apply() {
	if(isReady()) {
		cv::fastNlMeansDenoisingColored(*src, dst, *h, *hColor, *templateWindowSize, *searchWindowSize);
	}
}

/*
 * BackgroundImageFilter.cpp
 *
 *	See
 *
 * 	http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#backgroundsubtractormog2
 *
 * 	for details on the image filter.
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "BackgroundImageFilter.h"

/**
 * Constructor
 *
 * @param history: Length of the history.
 * @param varThreshold: Threshold on the squared Mahalanobis distance to decide whether it is well described by the background model (see Cthr??). This parameter does not affect the background update. A typical value could be 4 sigma, that is, varThreshold=4*4=16; (see Tb??).
 * @param bShadowDetection: Parameter defining whether shadow detection should be enabled.
 * @param supplyForeground: return foreground if true, background otherwise
 */
dsa::BackgroundImageFilter::BackgroundImageFilter(int history,  float varThreshold, bool bShadowDetection, bool supplyForeground) {
	supplyForeground_ = supplyForeground;
    bgs_ = cv::BackgroundSubtractorMOG2(history, varThreshold, bShadowDetection);

    init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BackgroundImageFilter::~BackgroundImageFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BackgroundImageFilter::init() {
	supplyForeground = &supplyForeground_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::BackgroundImageFilter::apply() {
	if(isReady()) {
	    std::vector<std::vector<cv::Point> > contours;
		bgs_(*src, foreground_);

		if(supplyForeground_)
			dst = foreground_.clone();
		else {
			bgs_.getBackgroundImage(background_);
			dst = background_.clone();
		}
	}
}

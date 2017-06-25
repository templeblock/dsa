/*
 * VisualizeBarcodeFilter.cpp
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

#include "VisualizeBarcodeFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::VisualizeBarcodeFilter::VisualizeBarcodeFilter(size_t maxContentsSize, int fontFace, double fontScale, cv::Scalar color, int thickness, int lineType) {
	maxContentsSize_ = maxContentsSize;
	fontFace_ = fontFace;
	fontScale_  = fontScale;
	color_ = color;
	thickness_ = thickness;
	lineType_ = lineType;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::VisualizeBarcodeFilter::~VisualizeBarcodeFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::VisualizeBarcodeFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::VisualizeBarcodeFilter::apply() {
	if(isReady()) {
		dst = *src;
		for(size_t i = 0; i < codes->size(); i++) {
			// mark region with ellipse
			cv::RotatedRect rrect = (*codes)[i].rrect;
			ellipse(dst, rrect, color_, thickness_, lineType_);

			if(maxContentsSize_ > 0) {	// write text into region
				cv::Rect rect = rrect.boundingRect();
				cv::Point anchor = rect.tl() + cv::Point(rect.width/10, rect.height/2);
				std::string *text = &((*codes)[i].contents);
				if(text->size() > maxContentsSize_)
					putText(dst, text->substr(0, maxContentsSize_-3).append("..."), anchor, fontFace_, fontScale_, color_);
				else
					putText(dst, *text, anchor, fontFace_, fontScale_, color_);
			}
		}
	}
}

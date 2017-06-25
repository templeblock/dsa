/*
 * GrabCutFilter.cpp
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#grabcut
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "GrabCutFilter.h"

/**
 * Constructor
 *
 * @param mask: Input/output 8-bit single-channel mask.
 * @param rect: ROI containing a segmented object. The pixels outside of the ROI are marked as “obvious background”. The parameter is only used when mode==GC_INIT_WITH_RECT.
 * @param bgdModel: Temporary array for the background model. Do not modify it while you are processing the same image.
 * @param fgdModel: Temporary arrays for the foreground model. Do not modify it while you are processing the same image.
 * @param iterCount: Number of iterations the algorithm should make before returning the result. Note that the result can be refined with further calls with mode==GC_INIT_WITH_MASK or mode==GC_EVAL.
 * @param mode: Operation mode that could be one of the following: GC_INIT_WITH_RECT The function initializes the state and the mask using the provided rectangle. After that it runs iterCount iterations of the algorithm. GC_INIT_WITH_MASK The function initializes the state using the provided mask. Note that GC_INIT_WITH_RECT and GC_INIT_WITH_MASK can be combined. Then, all the pixels outside of the ROI are automatically initialized with GC_BGD. GC_EVAL The value means that the algorithm should just resume.
 */
dsa::GrabCutFilter::GrabCutFilter(cv::Mat& mask, cv::Rect& rect, cv::Mat& bgdModel, cv::Mat& fgdModel, int iterCount, int mode) {
	mask_ = mask;
	rect_ = rect;
	bgdModel_ = bgdModel;
	fgdModel_ = fgdModel;
	iterCount_ = iterCount;
	mode_ = mode;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::GrabCutFilter::~GrabCutFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::GrabCutFilter::init() {
	mask = &mask_;
	rect = &rect_;
	bgdModel = &bgdModel_;
	fgdModel = &fgdModel_;
	iterCount = &iterCount_;
	mode = &mode_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::GrabCutFilter::evaluate() {
	grabCut(*src, *mask, *rect, *bgdModel, *fgdModel, *iterCount, *mode);
}

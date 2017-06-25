/*
 * MeanshiftFilter.cpp
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "MeanshiftFilter.h"

/**
 * Constructor
 *
 */
dsa::MeanshiftFilter::MeanshiftFilter(std::vector<cv::Mat> &histograms, std::vector<cv::Rect> &rois) {
	initialized_ = false;
	histograms_ = histograms;

	for(size_t i = 0; i < histograms_.size(); i++) {
		cv::normalize(histograms_[i], histograms_[i], 0, 255, cv::NORM_MINMAX);
		trackWindows_[i] = cv::Rect(rois[i]);
	}

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::MeanshiftFilter::~MeanshiftFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::MeanshiftFilter::init() {
	EvaluationFilter::init();
}

/**
 * Add to training data
 *
 */
void dsa::MeanshiftFilter::evaluate() {
	const float histRange[] = {0.0f, 255.0f};
	const float* phistRange = histRange;

	cv::TermCriteria criteria = cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1);
	for (size_t i = 0; i < histograms_.size(); i++) {
		cv::Mat bp;
		calcBackProject(src, 1, 0, histograms_[i], bp, &phistRange);
		cv::meanShift(bp, trackWindows_[i], criteria);
		cv::rectangle(dst, trackWindows_[i], cv::Scalar(255));
		//cv::imshow("Backprojection1", bp);
	}
}

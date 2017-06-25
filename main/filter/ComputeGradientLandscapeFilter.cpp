/*
 * ComputeGradientLandscapeFilter.cpp
 *
 * Compute the gradient landscape of an image
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "ComputeGradientLandscapeFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::ComputeGradientLandscapeFilter::ComputeGradientLandscapeFilter(size_t ksize, std::string &filename) {
	//outfile_.open(filename.c_str(), std::ios_base::out);
	ksize_ = ksize; // must be odd
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ComputeGradientLandscapeFilter::~ComputeGradientLandscapeFilter() {
	//outfile_.close();
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::ComputeGradientLandscapeFilter::init() {
	//outfile_ << "timestamp;x1;x2;x3;a1;a2;a3;" << std::endl;
	//outfile_ << "timestamp;x1;x2;a3;" << std::endl;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::ComputeGradientLandscapeFilter::evaluate() {
	minGradient = cv::Mat(src->rows, src->cols, CV_8UC1);
	maxGradient = cv::Mat(src->rows, src->cols, CV_8UC1);
	meanGradient = cv::Mat(src->rows, src->cols, CV_8UC1);
	medianGradient = cv::Mat(src->rows, src->cols, CV_8UC1);
	size_t offset = ksize_/2;
	size_t windowSize = ksize_*ksize_;
	UINT8 *flatWindow = new UINT8[windowSize];
	for(size_t i = offset; i < src->rows-offset; i++) {	// in all rows
		UINT8* srcrow = src->ptr<UINT8>(i);
		UINT8* minrow = minGradient.ptr<UINT8>(i);
	    UINT8* maxrow = maxGradient.ptr<UINT8>(i);
	    UINT8* meanrow = meanGradient.ptr<UINT8>(i);
	    UINT8* medianrow = medianGradient.ptr<UINT8>(i);
		for(size_t j = offset; j < src->cols-offset; j++) {	// in all cols
			int flatWindowIndex = 0;
			for(size_t k = i - offset; k < i + offset; k++) {
				UINT8* srcwinrow = src->ptr<UINT8>(k);
				for(size_t l = j - offset; l < j + offset; l++) {
					flatWindow[flatWindowIndex] = (UINT8)abs(srcwinrow[l]-srcrow[i]);
					flatWindowIndex++;
				}
			}
			std::sort(flatWindow, flatWindow+windowSize);
			minrow[j] = flatWindow[0];
			maxrow[j] = flatWindow[windowSize-1];
			medianrow[j] = flatWindow[(windowSize+1)/2];
			int mean = 0;
			for(size_t k = 0; k < windowSize; k++)
				mean += flatWindow[k];
			meanrow[j] = (UINT8)(mean/windowSize);
		}
	}
	delete flatWindow;
	imshow("minGradient", minGradient);
	imshow("maxGradient", maxGradient);
	imshow("meanGradient", meanGradient);
	imshow("medianGradient", medianGradient);
}

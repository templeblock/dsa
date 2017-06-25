/*
 * EqualizeHistFilter.cpp
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

#include "ImprovedEqualizeHistFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::ImprovedEqualizeHistFilter::ImprovedEqualizeHistFilter(int ksize, float centerWeight) {
	kernel_ = cv::Mat::ones(ksize, ksize, CV_32F)*((1.0f-centerWeight)/(float)(ksize*ksize-1));
	int  middle = ksize >> 1;
	kernel_.at<float>(middle, middle) = centerWeight;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ImprovedEqualizeHistFilter::~ImprovedEqualizeHistFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::ImprovedEqualizeHistFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function. This filter still needs a lot of performance improvement
 */
void dsa::ImprovedEqualizeHistFilter::apply() {
	if(isReady()) {
		cv::Mat tmp;
		// first weighted mean filter
		cv::filter2D(*src, tmp, CV_32F, kernel_);

		const int hist_sz = 256;
		int hist[hist_sz] = {0};
		float minmax[2] = {255.0f, 0.0f};
		std::vector<std::vector<float> *> meanValues(hist_sz, new std::vector<float>(minmax, minmax+2));
		size_t nPixels = src->rows * src->cols;

		// build histogram
		for(size_t i = 0; i < src->rows; i++) {
			UINT8 *srcRow = src->ptr<UINT8>(i);
			float *tmpRow = tmp.ptr<float>(i);
			for(size_t j = 0; j < src->cols; j++) {
				UINT8 pixelValue = srcRow[j];
				hist[pixelValue]++;
				// find min and max value in tmp that originated from pixelValue (so we can interpolate all others between)
				float &min = meanValues[pixelValue]->at(0);
				float &max = meanValues[pixelValue]->at(1);
				float meanValue = tmpRow[j];
				if(meanValue < min)
					min = meanValue;
				if(meanValue > max)
					max = meanValue;
			}
		}

		// build lookup table (aka cumulative distribution function)
		int sum = 0;
		float scale = 255.0f/(src->rows*src->cols);
		UINT8 lut[hist_sz+2];
		lut[0] = 0;
		for(size_t i = 1; i < hist_sz+1; i++) {
			sum += hist[i-1];
			lut[i] = cv::saturate_cast<UINT8>(round(sum*scale));
		}
		lut[1] = 0;
		lut[hist_sz+1] = lut[hist_sz];

		// set new values
		dst = cv::Mat(src->rows, src->cols, CV_8UC1);
		BorderCalculator bc = BorderCalculator(hist_sz);	// TODO: buffer calculated results (saves a lot of floating point devisions). A further optimization would just precalculate all.

		for(size_t i = 0; i < src->rows; i++) {
			UINT8 *srcRow = src->ptr<UINT8>(i);
			UINT8 *dstRow = dst.ptr<UINT8>(i);
			float *tmpRow = tmp.ptr<float>(i);
			for(size_t j = 0; j < src->cols; j++) {
				UINT16 pixelValue = srcRow[j];
				float minVal = meanValues[pixelValue]->at(0);	// the min mean value from this histogram column
				float maxVal = meanValues[pixelValue]->at(1);	// the max mean value from this histogram column
				float position = (tmpRow[j] - minVal)/(maxVal - minVal);	// tmp contains the mean values
				std::array<float, 2> intervalBorder = bc.getInterval(lut[pixelValue], lut[pixelValue+1], lut[pixelValue+2], pixelValue);

				dstRow[j] = (UINT8)(intervalBorder[0] + position * intervalBorder[1]);
			}
		}

		// cleanup
		for(size_t i = 0; i < meanValues.size(); i++)
			delete meanValues[i];
	}
}

dsa::ImprovedEqualizeHistFilter::BorderCalculator::BorderCalculator(size_t n) {
	bordersCalculated_ = std::vector<bool>(n, false);
	lowerBorder_ = std::vector<float>(n, 0.0f);
	intervalWidth_ = std::vector<float>(n, 0.0f);

	getIntervalArray[0] = &ImprovedEqualizeHistFilter::BorderCalculator::calcInterval;
	getIntervalArray[1] = &ImprovedEqualizeHistFilter::BorderCalculator::getIntervalFromBuffer;
}

dsa::ImprovedEqualizeHistFilter::BorderCalculator::~BorderCalculator() {
}

/**
 * Determine the interval beginning and width
 *
 * @param i1
 * @param i2
 * @param i3
 * @param pos
 * @return
 */
std::array<float, 2> dsa::ImprovedEqualizeHistFilter::BorderCalculator::calcInterval(UINT8 i1, UINT8 i2, UINT8 i3, UINT8 pos) {
	lowerBorder_[pos] = (float)((UINT32)i1 + i2)/2.0f;
	intervalWidth_[pos] = (float)((UINT32)i2 + i3)/2.0f - lowerBorder_[pos];
	bordersCalculated_[pos] = true;	// not necessary to set twice, so optimize this later
	std::array<float, 2> val = { { lowerBorder_[pos], intervalWidth_[pos] } };
	return(val);
}

/**
 * Dont calculate, but fetch precalculated info from buffer
 *
 * @param i1
 * @param i2
 * @param i3
 * @param pos
 * @return
 */
std::array<float, 2> dsa::ImprovedEqualizeHistFilter::BorderCalculator::getIntervalFromBuffer(UINT8 i1, UINT8 i2, UINT8 i3, UINT8 pos) {
	std::array<float, 2> val = { { lowerBorder_[pos], intervalWidth_[pos] } };
	return(val);
}

/**
 * Determine the interval we estimate the stretched values to come from during histogram equalization
 *
 * @param i1 previous
 * @param i2 current
 * @param i3 next
 * @param pos position in the buffer (the pixel value serves as indicator to the position within the arrays)
 *
 * @return
 */
std::array<float, 2> dsa::ImprovedEqualizeHistFilter::BorderCalculator::getInterval(UINT8 i1, UINT8 i2, UINT8 i3, UINT8 pos) {
	return((this->*getIntervalArray[bordersCalculated_[pos]])(i1, i2, i3, pos));
}

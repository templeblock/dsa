/*
 * ForkBackgroundFilter.cpp
 *
 * 	Filter background out from our view on the forks
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "ForkBackgroundFilter.h"
#include <iostream>

/**
 * Constructor
 *
 */
dsa::ForkBackgroundFilter::ForkBackgroundFilter(bool isColor) : ImageBufferFilter(1) {
	isColor_ = isColor;
	nruns_ = 0;
	prev_nruns_ = 0;
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ForkBackgroundFilter::~ForkBackgroundFilter() {
}

void dsa::ForkBackgroundFilter::evaluate() {
	prev_nruns_ = nruns_;
	cv::Mat tmp;
	if(isColor_) {
		tmp.create(src->size(), src->depth());
		int vch[] = { 2, 0 };
		mixChannels(src, 1, &tmp, 1, vch, 1);
	} else
		tmp = src->clone();

	if(buffer_.isEmpty()) {
		reset();
		buffer_.write(tmp);
	} else {
		int nPixels = tmp.rows * tmp.cols;
		// tmp2 contains the difference between buffered image and the normalized image tmp

		cv::Mat bufferedMat = *(buffer_.read());
		cv::absdiff(tmp, bufferedMat, tmp);
		cv::subtract(tmp, cv::Scalar(25), tmp);

		int nonZeroPixels = countNonZero(tmp);
		double nonZeroRatio = (double)nonZeroPixels/nPixels;
		double frameThreshold = 0.30;
		const float pixelThresholdRatio = 0.32f;	// mark the 32% percent brightest pixel
		//std::cout << "forkbackgroundfilter: " << nonZeroRatio << "," << frameThreshold << std::endl;
		if(nonZeroRatio > frameThreshold) {	// only do the computations if we have a big difference between the images (saves computation time)
			// compute histogram
			const float rangeMin = 0.0f;
			const float rangeMax = 255.0f;
			const float range[] = {rangeMin, rangeMax};
			const float *histRange = {range};
			const int histSize = 255;
			cv::Mat hist;
			cv::calcHist(&tmp, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false);

			int pixelThreshold = 1;
			float sum = 0.0f;
			while((sum += hist.at<float>(pixelThreshold)) < (1.0f-pixelThresholdRatio)*nonZeroPixels)
				pixelThreshold++;

			//std::cout << "PixelThreshold = " << pixelThreshold << std::endl;

			// for every pixel check if difference is > pixelThreshold
			if(isColor_) {
				for(int j = 0; j < src->rows; j++) {
					for(int i = 0; i < src->cols; i++) {
						average_.at<cv::Vec3i>(j,i) += src->at<cv::Vec3i>(j,i);
						if(tmp.at<UINT8>(j,i) > pixelThreshold)
							image_.at<UINT8>(j,i) = 255; // make background white
					}
				}
			} else {
				for(int j = 0; j < src->rows; j++) {
					for(int i = 0; i < src->cols; i++) {
						average_.at<int>(j,i) += src->at<UINT8>(j,i);
						if(tmp.at<UINT8>(j,i) > pixelThreshold)
							image_.at<UINT8>(j,i) = 255; // make background white
					}
				}
			}
			buffer_.write(tmp);
			nruns_++;
		}

		dst = image_.clone();
	}

	cv::putText(dst, intToString(nruns_), cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(127));
}

void dsa::ForkBackgroundFilter::reset() {
	nruns_ = 0;
	image_ = cv::Mat::zeros(src->rows, src->cols, CV_8UC1); // image is initially all black
	if(isColor_)
		average_ = cv::Mat::zeros(src->rows, src->cols, CV_32SC3); // image is initially all black
	else
		average_ = cv::Mat::zeros(src->rows, src->cols, CV_32SC1); // image is initially all black
}

bool dsa::ForkBackgroundFilter::usedLastImage() {
	return(prev_nruns_ < nruns_);
}

cv::Mat dsa::ForkBackgroundFilter::getAverage() {
	cv::Mat returnValue;
	if(isColor_)
		returnValue = cv::Mat(src->rows, src->cols, CV_8UC3);
	else
		returnValue = cv::Mat(src->rows, src->cols, CV_8UC1);
	convertScaleAbs(average_, returnValue, 1.0/nruns_);
	return(returnValue);
}

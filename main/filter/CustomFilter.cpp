/*
 * CustomFilter.cpp
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "CustomFilter.h"
#include <iostream>

/**
 *
 * @param logfileName
 * @param area
 * @param maskedPixelsPerLine
 * @param nbins number of bins. Will be multiplied by 3
 */
dsa::CustomFilter::CustomFilter(std::string logfileName, double area, std::vector<size_t> *maskedPixelsPerLine, cv::Mat mask, size_t nbins) {
	logfileName_ = logfileName.append(".csv");
	area_ = area;
	maskedPixelsPerLine_ = maskedPixelsPerLine;
	mask_ = mask;
	nbins_ = nbins;

	state_ = unknown;
	fae_ = ForkAllocationEvent(30);

	if(logfileName_.length() > 0) {
		try {
			logfile_.open(logfileName_.c_str());
			// table headlines
			logfile_ << "\"total mean\";\"total mean difference\";\"state\";";
			for(int i = 0; i < nbins_; i++)
				logfile_ << "\"binMean[" << i << "]\";";
			for(int i = 0; i < nbins_; i++)
				logfile_ << "\"binDiff[" << i << "]\";";

			logfile_ << std::endl;
		} catch (std::ofstream::failure &failure) {
			std::cout << "Error opening logfile " << logfileName_ << ". Error was:" << std::endl;
			std::cout << failure.what() << std::endl;
		}
	}

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::CustomFilter::~CustomFilter() {
	logfile_.close();
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::CustomFilter::init() {
	EvaluationFilter::init();
	state = &state_;
}

/**
 * Determine Fork
 *
 */
void dsa::CustomFilter::evaluate() {
	const size_t margin = 0.02*(src->rows+src->cols); // margin of the forkmask (in case the fork moves relative to camera through vibrations)
	const size_t ksize = 3;	// kernel size
	const size_t offset = margin + ksize/2;

	// characteristics of the bins
	// determine rows to sample
	size_t firstRow = 0.05*src->rows;	// skip first 5%
	size_t lastRow = src->rows - ksize/2 - 1; // we need the full operator window
	size_t rowsPerBin = (lastRow-firstRow+1)/nbins_;

	// mean value
	std::vector<int> binPixels = std::vector<int>(nbins_, 0);
	std::vector<double> binMean(nbins_, 0);
	for(size_t i = firstRow; i <= lastRow; i++) {	// in all rows
		int currentBin = (i-firstRow)/rowsPerBin;
		if(currentBin < nbins_) { // skip leftovers
			for(size_t j = margin; j < src->cols-margin; j++) {	// in all cols
				if(mask_.at<uchar>(i, j) > 0 && mask_.at<uchar>(i, j-margin) > 0 && mask_.at<uchar>(i, j+margin) > 0) { // consider only pixel within margin
					binMean[currentBin] += src->at<uchar>(i,j); // front
					binPixels[currentBin]++;
					//dst.at<uchar>(i,j) = 255;
				}
			}
		}
	}
	for(size_t i = 0; i < nbins_; i++) {
		if(binPixels[i] > 0)
			binMean[i] /= binPixels[i];
	}

	// max local difference
	binPixels = std::vector<int>(nbins_, 0);
	std::vector<double> binDifference(nbins_, 0.0);
	for(size_t i = firstRow; i <= lastRow; i+=(ksize-1)) {	// for all rows
		int currentBin = (i-firstRow)/rowsPerBin;
		if(currentBin < nbins_) {	// skip leftovers
			for(size_t j = offset; j < src->cols-offset; j+=(ksize-1)) {	// in all cols
				if(mask_.at<uchar>(i, j) > 0 && mask_.at<uchar>(i, j-offset) > 0 && mask_.at<uchar>(i, j+offset) > 0) { // consider only pixel within margin
					int maxDiff = 0;
					int value = src->at<uchar>(i,j);
					for(size_t y = i-ksize/2; y <= i+ksize/2; y++)	// operator window
						for(size_t x = j-ksize/2; x <= j+ksize/2; x++)
							if(mask_.at<uchar>(y,x) > 0)
								maxDiff = std::max(maxDiff, abs(src->at<uchar>(y,x)-value));	// add maximum local difference

					binDifference[currentBin] += maxDiff;
					binPixels[currentBin]++;
				}
			}
		}
	}
	for(size_t i = 0; i < nbins_; i++) {
		if(binPixels[i] > 0)
			binDifference[i] /= binPixels[i];
	}

	state_ = fae_.calcState(binMean, binDifference);
	std::vector<double> meanValues = fae_.getMean();
	if(logfileName_.length() > 0) {
		//std::vector<float> moduli = computeHistogramModuli(*src, 255, 1.0f, 255.0f);
		logfile_ << meanValues[0] << ";" << meanValues[1] << ";" << state_ << ";";
		for(int i = 0; i < nbins_; i++)
			logfile_ << binMean[i] << ";";
		for(int i = 0; i < nbins_; i++)
			logfile_ << binDifference[i] << ";";
		logfile_ << std::endl;
	}
}

/**
 * Update area and maskedPixelsPerLine
 *
 * @param area
 * @param maskedPixelsPerLine
 */
void dsa::CustomFilter::update(double area, std::vector<size_t> *maskedPixelsPerLine, cv::Mat mask) {
	area_ = area;
	maskedPixelsPerLine_ = maskedPixelsPerLine;
	mask_ = mask;
	fae_.reset();
}

/**
 * Computes the histogram of an image and determines its common moduli (min, max, mean, stddev)
 *
 * @param mat
 * @param histSize
 * @param rangeMin
 * @param rangeMax
 * @return
 */
std::vector<float> dsa::CustomFilter::computeHistogramModuli(cv::Mat &mat, const int histSize, const float rangeMin, const float rangeMax) {
	const float range[] = {rangeMin, rangeMax} ;
	const float *histRange = {range};

	// compute histogram
	cv::Mat hist;
	cv::calcHist(&mat, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false);
	delete histRange;

	cv::Scalar mean, stddev;
	double min, max;
	cv::meanStdDev(hist, mean, stddev);
	cv::minMaxLoc(hist, &min, &max);

	// modified shannon entropy
	float frequency = 0.0f;	// how many pixels to consider
	for(int i = 1; i < histSize; i++)
		frequency += fabsf(hist.at<float>(i));

	// entropy = -sum((p_xi)*log(p_xi))
	float entropy = 0.0f;
	for(int i = 1; i < histSize; i++) {	// skip total black and total white
		float p_xi = fabsf(hist.at<float>(i))/frequency;	// probability of a pixel being of color value i
		if(p_xi > 0) // log is only defined for p_xi > 0
			entropy += p_xi * log10f(1.0f+p_xi);
	}

	std::vector<float> retval = std::vector<float>();
	retval.push_back((float)min);
	retval.push_back((float)max);
	retval.push_back((float)mean.val[0]);
	retval.push_back((float)stddev.val[0]);
	retval.push_back(entropy);

	return(retval);
}


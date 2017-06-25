/*
 * ForkChangeFilter.cpp
 *
 * 	Filter background out from our view on the forks
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "ForkChangeFilter.h"

/**
 * Constructor
 *
 */
dsa::ForkChangeFilter::ForkChangeFilter(std::string logfileName) : ImageBufferFilter(15) {
	nruns_ = 0;
	logfileName_ = logfileName.append(".csv");

	if(logfileName_.length() > 0) {
		try {
			logfile_.open(logfileName_.c_str());
			// table headlines
			logfile_ << "\"index\";\"nonZero\";\"difference1\";\"difference2\";" << std::endl;
		} catch (std::ofstream::failure &failure) {
			std::cout << "Error opening logfile " << logfileName_ << ". Error was:" << std::endl;
			std::cout << failure.what() << std::endl;
		}
	}
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ForkChangeFilter::~ForkChangeFilter() {
}

void dsa::ForkChangeFilter::evaluate() {
	if(buffer_.isEmpty()) {
		reset();
		buffer_.write(*src);
	} else {
		const cv::Scalar minPixelChange = cv::Scalar(16);
		int nPixels = src->rows * src->cols;
		cv::Scalar mean = cv::mean(*src);
		cv::Mat bufferedMat = *(buffer_.read());
		cv::Mat tmp = src->clone();	// create copy to work on
		cv::absdiff(tmp, bufferedMat, tmp);	// difference between current and buffered
		cv::subtract(tmp, minPixelChange, tmp);	// subtract (saturized) a threshold

		// moduli
		double nonZeroPixels = countNonZero(tmp);	// compute number of pixel that changed by at least minPixelChange
		cv::Scalar difference = cv::sum(tmp);	// compute overall difference and normalize to mean value

		double val1 = nonZeroPixels/nPixels;
		double val2 = difference.val[0]/mean.val[0];
		double val3 = difference.val[0]/nPixels;

		imshow("tmp", tmp);
		if(cv::waitKey(10) == 'q')
			exit(1);
		//cv::waitKey(0);

		logfile_ << nruns_ << ";" << val1 << ";" << val2 << ";" << val3 << ";" << std::endl;
		buffer_.write(*src);
		nruns_++;
	}
}

void dsa::ForkChangeFilter::reset() {
	buffer_.free();
	nruns_ = 0;
}

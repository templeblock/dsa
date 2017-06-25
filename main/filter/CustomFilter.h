/*
 * CustomFilter.h
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef CUSTOMFILTER_H_
#define CUSTOMFILTER_H_

#include <algorithm>
#include <fstream>
#include <cmath>
#include <generic/types.h>
#include <generic/helper.h>
#include <generic/MeanQueueBuffer.h>
#include <demonstrator/ForkAllocationEvent.h>
#include "EvaluationFilter.h"

namespace dsa {

class CustomFilter : public EvaluationFilter {
	int nbins_;
	double area_;
	ForkState state_;
	std::string logfileName_;
	std::ofstream logfile_;

	cv::Mat mask_;
	ForkAllocationEvent fae_;
	std::vector<size_t>* maskedPixelsPerLine_;

	void init();
	void evaluate();

	std::vector<float> computeHistogramModuli(cv::Mat &mat, const int histSize, const float rangeMin, const float rangeMax);

public:

	ForkState* state;

	CustomFilter(std::string logfileName, double area, std::vector<size_t> *maskedPixelsPerLine, cv::Mat mask, size_t nbins=10);
	virtual ~CustomFilter();

	void update(double area, std::vector<size_t> *maskedPixelsPerLine, cv::Mat mask);

	float getHistogramBinValue(cv::Mat hist, int binNum);
};

}
#endif /* CUSTOMFILTER_H_ */

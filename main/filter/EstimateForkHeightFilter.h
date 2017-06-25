/*
 * EstimateForkHeightFilter.h
 *
 *	This filter estimates the fork height.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef ESTIMATEFORKHEIGHTFILTER_H_
#define ESTIMATEFORKHEIGHTFILTER_H_

#include <cmath>
#include <generic/helper.h>
#include <generic/MeanQueueBuffer.h>
#include "EvaluationFilter.h"
#include <demonstrator/localizor/Settings.hpp>
#include <demonstrator/localizor/LocalizorMulti.hpp>
#include <demonstrator/localizor/ExponentialSmoothing.hpp>

namespace dsa {

class EstimateForkHeightFilter : public EvaluationFilter {

	double factor_;	// perimeter to height relation is: factor_ * perimeter^exponent_
	double exponent_;
	Settings settings_;
	ExponentialSmoothing smoothPos_;
	MeanQueueBuffer<float> buf_;

	void init();
	void evaluate();

public:

	bool haveValue;
	std::string text;

	cv::Mat camPosition;
	cv::Mat camRotation;

	std::vector<aruco::Marker> *markers;

	EstimateForkHeightFilter(Settings &settings);
	virtual ~EstimateForkHeightFilter();

	void preprocessing();
};

}
#endif /* ESTIMATEFORKHEIGHTFILTER_H_ */

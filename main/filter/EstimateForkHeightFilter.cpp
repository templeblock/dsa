/*
 * EstimateForkHeightFilter.h
 *
 *	This filter extracts AR Markers from camera images using the aruco library.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "EstimateForkHeightFilter.h"

/**
 * Constructor
 */
dsa::EstimateForkHeightFilter::EstimateForkHeightFilter(Settings &settings) {
	haveValue = false;
	text = std::string("");

	settings_ = settings;
	factor_ = settings_.heightFormulaFactor;
	exponent_ = settings_.heightFormulaExponent;

	smoothPos_ = ExponentialSmoothing(settings_.exponentialSmoothingAlpha);

	// it depends on the use case which methods work best, so the settings can be overruled
	camPosition = cv::Mat::zeros(3, 1, CV_32FC1);
	camRotation = cv::Mat::zeros(3, 1, CV_32FC1);

	buf_ = MeanQueueBuffer<float>(60);

	init();
}

/**
 * Destructor
 */
dsa::EstimateForkHeightFilter::~EstimateForkHeightFilter() {
}

void dsa::EstimateForkHeightFilter::init() {
}

void dsa::EstimateForkHeightFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	text = std::string("(no detection)");
}

/**
 * Fill internal data structure with markers
 */
void dsa::EstimateForkHeightFilter::evaluate() {
	if(markers->size() > 0) {
		haveValue = true;

		aruco::Marker m = (*markers)[0];
		// estimate height based on perimeter of the marker
		double perimeter = m.getPerimeter();
		buf_.write(perimeter);
		float height = factor_ * pow(perimeter, exponent_);
		//std::cout << "perimeter=" << buf_.getMean() << std::endl;

		camPosition.at<float>(0,2) = height;
		smoothPos_.add(camPosition);
		smoothPos_.get(camPosition);

		text = std::string("t=(0, 0, ");
		text.append(doubleToString((int)camPosition.at<float>(0, 2)));
		text.append("); r=(0, 0, 0);");
	} else {
		haveValue = false;
	}
}

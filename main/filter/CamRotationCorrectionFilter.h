/*
 * CamRotationCorrectionFilter.h
 *
 *  Created on: 28.08.2013
 *      Author: Messen
 */


#ifndef CAMROTATIONCORRECTIONFILTER_H_
#define CAMROTATIONCORRECTIONFILTER_H_

#include <cmath>
#include <generic/helper.h>
#include "EvaluationFilter.h"
#include <demonstrator/localizor/Settings.hpp>
#include <demonstrator/localizor/MarkerPose.hpp>

namespace dsa {
/*
 * Rotates the 2D-Points of a vector of markers, as specified by a rotation vector.
 *
 * input:
 *  - markers (as vector<aruco::Marker>)
 *  - camRotation (as 3-Vector)
 *
 * output:
 *  - manipulatedMarkers (as vector<aruco::Marker>)
 */
class CamRotationCorrectionFilter : public EvaluationFilter {
	// by constructor:
	Settings settings_;
	cv::Mat camRotation_;

	void init();
	void evaluate();

public:
	//output:
	std::vector<aruco::Marker> manipulatedMarkers;

	// input:
	std::vector<aruco::Marker> *markers;
	cv::Mat *camRotation;

	CamRotationCorrectionFilter(Settings &settings);
	CamRotationCorrectionFilter(Settings &settings, cv::Mat &camRotation);
	virtual ~CamRotationCorrectionFilter();

	void preprocessing();
};

}
#endif /* CAMROTATIONCORRECTIONFILTER_H_ */




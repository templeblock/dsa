/*
 * EstimatePoseFilter.h
 *
 *	This filter extracts AR Markers from camera images using the aruco library.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef ESTIMATEPOSEFILTER_H_
#define ESTIMATEPOSEFILTER_H_

#include <cmath>
#include <generic/helper.h>
#include "EvaluationFilter.h"
#include <demonstrator/localizor/Settings.hpp>
#include <demonstrator/localizor/LocalizorMulti.hpp>
#include <demonstrator/localizor/ExponentialSmoothing.hpp>

namespace dsa {

class EstimatePoseFilter : public EvaluationFilter {

	int transCoords_;
	int rotCoords_;

	Settings settings_;
	LocalizorMulti locmulti_;
	ExponentialSmoothing smoothPos_;
	ExponentialSmoothing smoothRot_;

	//std::vector<aruco::Marker> markers_;
	//std::vector<MarkerPose> poses_;

	void init();
	void evaluate();

public:

	bool haveValue;
	std::string text;

	cv::Mat camPosition;
	cv::Mat camRotation;

	std::vector<aruco::Marker> *markers;
	std::vector<MarkerPose> *poses;

	EstimatePoseFilter(Settings &settings, int transCoords=3, int rotCoords=4, int poseEstAlgoOverride=0, int regulationStrategyOverride=0);
	// sum of the coordinates used for translation and rotation: x=1, y=2, z=4. default values are for localization in x,y plane
	// poseEstAlgo/regulationStrategy == 0 -> use values from settings-file
	virtual ~EstimatePoseFilter();

	void preprocessing();
};

}
#endif /* ESTIMATEPOSEFILTER_H_ */

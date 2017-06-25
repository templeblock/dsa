/*
 * EmjaysFancyForkPoseFilter.h
 *
 *  Created on: 21.08.2013
 *      Author: Messen
 */

#ifndef EMJAYSFANCYFORKPOSEFILTER_H_
#define EMJAYSFANCYFORKPOSEFILTER_H_

#include <cmath>
#include <generic/helper.h>
#include "EvaluationFilter.h"
#include "Settings.hpp"

namespace dsa {

/*
 * Calculates the height of the fork and its rotation.
 * Input:
 *  - Camera-Pose of height marker
 *  - Rotation marker (as aruco::Marker object)
 *
 * The measurements of the forklift are stored in a separate settings file.
 */
class EmjaysFancyForkPoseFilter : public EvaluationFilter {

	bool smoothInitialized_;
	cv::Point2f C;
	cv::Point2f R;
	cv::Point2f M_R;
	float forkTipDistance_;
	float heightMarkerOffset_;
	double b_1byb_2;
	//float markerAxisAngle_;
	Settings settings_;
	double initialCamRotation_;
	double rotationOffset_;
	double heightOffset_;
	double alpha_2;
	double delta_2;
	double smoothAlpha_;

	double x_min;	// measure with calibration (average)
	double x_max;	// measure with calibration (average)
	double y_min;	// measure with calibration (average)
	double y_max;	// measure with calibration (average)
	double imagePathLength;
	double completeAngle;
	double zeroAngleOffset;

	void evaluate();

public:

	bool haveValue;
	// input:
	cv::Mat *measuredHeightMarkerPosition;
	vector<aruco::Marker>* markers;

	// output
	cv::Point2f forkEndPosition;
	float forkRotation;
	float forkHeight;
	cv::Mat camRotation;
	std::string text;

	EmjaysFancyForkPoseFilter(Settings &settings);
	virtual ~EmjaysFancyForkPoseFilter();

	void preprocessing();
};

}
#endif /* EMJAYSFANCYFORKPOSEFILTER_H_ */

/*
 * ForkPoseFilter.h
 *
 *  Created on: 21.08.2013
 *      Author: Messen
 */

#ifndef FORKPOSEFILTER_H_
#define FORKPOSEFILTER_H_

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
class ForkPoseFilter : public EvaluationFilter {

	bool smoothInitialized_;
	cv::Point2f cameraPosition_;
	cv::Point2f rotationAxisPosition_;
	float forkTipDistance_;
	float heightMarkerOffset_;
	float markerAxisDist_;
	float axisCamDist_;
	float markerAxisAngle_;
	Settings settings_;
	float initialCamRotation_;
	float angleCam_;
	float smoothAlpha_;

	void evaluate();

public:

	// input:
	cv::Mat *measuredHeightMarkerPosition;
	vector<aruco::Marker>* markers;

	// output
	cv::Point2f forkEndPosition;
	float forkRotation;
	float forkHeight;
	cv::Mat camRotation;
	std::string text;

	ForkPoseFilter(Settings &settings);
	virtual ~ForkPoseFilter();

	void preprocessing();
};

}
#endif /* FORKPOSEFILTER_H_ */

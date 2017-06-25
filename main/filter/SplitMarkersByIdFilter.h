/*
 * SplitMarkersByIdFilter.h
 *
 *	This filter extracts AR Markers from camera images using the aruco library.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef SPLITMARKERSBYIDFILTER_H_
#define SPLITMARKERSBYIDFILTER_H_

#include <generic/types.h>
#include "EvaluationFilter.h"
#include <demonstrator/localizor/MarkerUtils.hpp>
#include <demonstrator/localizor/MarkerPose.hpp>
#include <demonstrator/localizor/MarkerStorage.hpp>

namespace dsa {

class SplitMarkersByIdFilter : public EvaluationFilter {

	MarkerStorage markerStorage_location_;
	// The marker which is used to estimate the height of the fork:
	MarkerStorage markerStorage_height_;
	// The marker which is used to estimate the rotation of the fork:
	MarkerStorage markerStorage_rotation_;
	std::vector<aruco::Marker> markers_;

	void init();
	void evaluate();

public:

	std::vector<aruco::Marker> *markers;

	std::vector<aruco::Marker> location_markers;
	std::vector<aruco::Marker> height_markers;
	std::vector<aruco::Marker> rotation_markers;
	std::vector<MarkerPose> location_poses;
	std::vector<MarkerPose> height_poses;
	std::vector<MarkerPose> rotation_poses;

	SplitMarkersByIdFilter(std::string &config);
	virtual ~SplitMarkersByIdFilter();

	void preprocessing();
};

}
#endif /* SPLITMARKERSBYIDFILTER_H_ */

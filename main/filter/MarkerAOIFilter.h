/*
 * MarkerAOIFilter.h
 *
 *	This filter extracts AR Markers from camera images using the aruco library.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef MARKERAOIFILTER_H_
#define MARKERAOIFILTER_H_

#include "EvaluationFilter.h"
#include <generic/helper.h>
#include <aruco.h>

namespace dsa {

class MarkerAOIFilter : public EvaluationFilter {

	float factor_;
	std::vector<aruco::Marker> markers_;

	void evaluate();

public:

	bool haveMarker;
	cv::Rect cutoff;
	std::vector<Circle> aois;
	std::vector<aruco::Marker>* markers;

	MarkerAOIFilter(const float factor=1.0f);
	virtual ~MarkerAOIFilter();

	void preprocessing();
};

}
#endif /* MARKERAOIFILTER_H_ */

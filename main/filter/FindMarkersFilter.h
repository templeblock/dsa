/*
 * FindMarkersFilter.h
 *
 *	This filter extracts AR Markers from camera images using the aruco library.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef FINDMARKERSFILTER_H_
#define FINDMARKERSFILTER_H_

#include <list>
#include <map>
#include <generic/types.h>
#include <generic/VectorPredictor.h>
#include "EvaluationFilter.h"
#include <demonstrator/localizor/MarkerUtils.hpp>
#include <demonstrator/localizor/MarkerPose.hpp>
#include <demonstrator/localizor/Settings.hpp>
#include <aruco.h>

namespace dsa {

class FindMarkersFilter : public EvaluationFilter {

	Settings settings_;
	//std::map<int, VectorPredictor<MathVector<int> > predMap;
	//std::list<int> foundIds;

	void evaluate();

public:

	std::vector<aruco::Marker> markers;

	FindMarkersFilter(Settings &settings);
	virtual ~FindMarkersFilter();

	void preprocessing();
	void postprocessing();
};

}
#endif /* FINDMARKERSFILTER_H_ */

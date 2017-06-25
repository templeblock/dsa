/*
 * FindMarkersFilter.h
 *
 *	This filter extracts AR Markers from camera images using the aruco library.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "FindMarkersFilter.h"

/**
 * Constructor
 */
dsa::FindMarkersFilter::FindMarkersFilter(Settings &settings) {
	settings_ = settings;

//	// construct predictors for all configured markers
//	MarkerStorage markerStorage_location = MarkerStorage("config/"+config+"/markers_location.yaml");
//	markerStorage_location.load();
//	vector<MarkerPose*> poses;
//	markerStorage_location.getAll(poses);
//
//	predMap = std::map<int, VectorPredictor<MathVector<int> >();
//	for(size_t i = 0; i < poses.size(); i++)
//		predMap[poses[i]->id] = VectorPredictor<MathVector<int> >();

	markers = std::vector<aruco::Marker>();
}

/**
 * Destructor
 */
dsa::FindMarkersFilter::~FindMarkersFilter() {

}

void dsa::FindMarkersFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	markers.clear();	
	
//	foundIds.clear();
}

void dsa::FindMarkersFilter::postprocessing() {
	EvaluationFilter::postprocessing();
	
//	for(size_t i = 0; i < markers.size(); i++) {
//		foundIds.push_back(markers[i].id);
//		cv::Point2f center = markers[i].getCenter();
//		std::vector<int> elementVector = std::vector<int>();
//		elementVector.push_back(center.x);
//		elementVector.push_back(center.y);
//		MathVector<int> v = MathVector<int>(elementVector);
//		predMap[markers[i].id].feed(v); // update
//	}
}

/**
 * Fill internal data structure with markers
 */
void dsa::FindMarkersFilter::evaluate() {
	findMarkers(dst, markers, settings_.camParameters, settings_.detectionThresholdBlockSize, settings_.detectionThresholdSubtraction, settings_.detectionMinMarker, settings_.detectionMaxMarker);
}

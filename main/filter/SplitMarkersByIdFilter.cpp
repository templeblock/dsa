/*
 * SplitMarkersByIdFilter.h
 *
 *	This filter extracts AR Markers from camera images using the aruco library.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "SplitMarkersByIdFilter.h"

/**
 * Constructor
 */
dsa::SplitMarkersByIdFilter::SplitMarkersByIdFilter(std::string &config) {
	markerStorage_location_ = MarkerStorage("config/"+config+"/markers_location.yaml");
	markerStorage_height_ = MarkerStorage("config/"+config+"/markers_height.yaml");
	markerStorage_rotation_ = MarkerStorage("config/"+config+"/markers_rotation.yaml");
	markerStorage_location_.load();
	markerStorage_height_.load();
	markerStorage_rotation_.load();

	markers_ = 	std::vector<aruco::Marker>();
	location_markers = std::vector<aruco::Marker>();
	height_markers = std::vector<aruco::Marker>();
	rotation_markers = std::vector<aruco::Marker>();
	location_poses = std::vector<MarkerPose>();
	height_poses = std::vector<MarkerPose>();
	rotation_poses = std::vector<MarkerPose>();

	init();
}

/**
 * Destructor
 */
dsa::SplitMarkersByIdFilter::~SplitMarkersByIdFilter() {
}

void dsa::SplitMarkersByIdFilter::init() {
	//markers = &markers_;
}

void dsa::SplitMarkersByIdFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	location_markers.clear();
	location_poses.clear();
	height_markers.clear();
	height_poses.clear();
	rotation_markers.clear();
	rotation_poses.clear();
	
}

/**
 * Fill internal data structure with markers
 */
void dsa::SplitMarkersByIdFilter::evaluate() {	   
  
   
	for(size_t i = 0; i < markers->size(); i++) {
		if(markerStorage_location_.contains((*markers)[i].id)) {	// location marker
			location_markers.push_back((*markers)[i]);
			location_poses.push_back(*(markerStorage_location_.getInfo((*markers)[i].id)));
			(*markers)[i].draw(dst, Scalar(0,255,0), 2, true);
		} else if(markerStorage_height_.contains((*markers)[i].id)) {	// fork height marker
			height_markers.push_back((*markers)[i]);
			height_poses.push_back(*markerStorage_height_.getInfo((*markers)[i].id));
			(*markers)[i].draw(dst, Scalar(0,0,255), 2, true);
		} else if(markerStorage_rotation_.contains((*markers)[i].id)) { // fork rotation marker
			rotation_markers.push_back((*markers)[i]);
			rotation_poses.push_back(*markerStorage_rotation_.getInfo((*markers)[i].id));
			(*markers)[i].draw(dst, Scalar(0,128,128), 2, true);
		} else {
			(*markers)[i].draw(dst, Scalar(255,0,0), 2, true);
			// The detected id is not used, it must have been an incorrect detection
			//markers.erase(markers.begin()+i);
			//i--;
		}
	}	
	
}

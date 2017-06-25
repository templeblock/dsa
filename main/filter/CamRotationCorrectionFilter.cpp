/*
 * CamRotationCorrectionFilter.cpp
 *
 *  Created on: 28.08.2013
 *      Author: Messen
 */


#include "CamRotationCorrectionFilter.h"

/**
 * Constructor
 */
dsa::CamRotationCorrectionFilter::CamRotationCorrectionFilter(Settings &settings) {
	settings_ = settings;

	init();
}

/**
 * Constructor
 */
dsa::CamRotationCorrectionFilter::CamRotationCorrectionFilter(Settings &settings, cv::Mat &camRotation) {
	settings_ = settings;
	camRotation_ = camRotation;
	this->camRotation = &camRotation_;

	init();
}

/**
 * Destructor
 */
dsa::CamRotationCorrectionFilter::~CamRotationCorrectionFilter() {
}

void dsa::CamRotationCorrectionFilter::init() {
}

void dsa::CamRotationCorrectionFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	manipulatedMarkers.clear();
}

/**
 * The idea is to rotate every corner point of every marker
 */
void dsa::CamRotationCorrectionFilter::evaluate() {
	// Read relevant camera parameters
	float fx = settings_.camParameters.CameraMatrix.at<float>(0,0);
	float fy = settings_.camParameters.CameraMatrix.at<float>(1,1);
	float cx = settings_.camParameters.CameraMatrix.at<float>(0,2);
	float cy = settings_.camParameters.CameraMatrix.at<float>(1,2);

	// perform a transformation for every point of every marker
	for(int i=0; i<markers->size(); i++) {
		vector<Point2f> corners;
		for(int j=0; j<(*markers)[i].size(); j++) { // for every point
			// distance to center:
			float dx = (*markers)[i][j].x - cx;
			float dy = (*markers)[i][j].y - cy;

			float f = (dx*fx + dy*fy)/(dx + dy);

			// imagine a line which goes through the camera
			//   and the point of the marker at the ceiling
			// This line also has to pass through the following point v:
			// (The camera position is the origin here)
			cv::Mat v = cv::Mat(3,1,CV_32FC1);
			v.at<float>(0) = dx;
			v.at<float>(1) = dy;
			v.at<float>(2) = f;

			// convert rotation vector to rotation matrix:
			cv::Mat R;
			cv::Rodrigues(*camRotation, R);

			// rotate point v
			v = R*v;
			// now the line through camera and point v has moved;
			// so calculate the new intersection with the image plane:
			v = v * (f / v.at<float>(2));

			// collect the corners
			// v is relative to image center
			corners.push_back(Point2f(cx + v.at<float>(0), cy + v.at<float>(1)));
		}
		// create new markers so that the old ones stay unchanged
		aruco::Marker m = aruco::Marker(corners, (*markers)[i].id);
		manipulatedMarkers.push_back(m);
		//m.draw(this->dst, cv::Scalar(255,0,0),4,true);
	}
}

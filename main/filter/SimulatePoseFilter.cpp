/*
 * SimulatePoseFilter.h
 *
 *	This filter simulates localization information for testing purposes.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "SimulatePoseFilter.h"

/**
 * Constructor
 */
dsa::SimulatePoseFilter::SimulatePoseFilter(int transCoords, int rotCoords) {
	transCoords_ = transCoords;
	rotCoords_ = rotCoords;

	camPosition = cv::Mat::ones(3, 1, CV_32FC1)*-FLT_MAX;
	camRotation = cv::Mat::ones(3, 1, CV_32FC1)*-FLT_MAX;
	timestamp = 0;

	forkHeight = 0;
	tiltAngle = 90;
	forkOccupied1 = 0;
	forkOccupied2 = -1;

	x=0, y=0, r=-PI;

	init();
}

/**
 * Destructor
 */
dsa::SimulatePoseFilter::~SimulatePoseFilter() {
}

bool dsa::SimulatePoseFilter::isReady() const {
	return(true);
}

void dsa::SimulatePoseFilter::init() {
}

void dsa::SimulatePoseFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	camPosition = cv::Mat::ones(3, 1, CV_32FC1)*-FLT_MAX;
	camRotation = cv::Mat::ones(3, 1, CV_32FC1)*-FLT_MAX;
}

/**
 * Fill internal data structure with markers
 */
void dsa::SimulatePoseFilter::evaluate() {
	// simulate random pose
	if(x < 1000 && y <= 0) {
		forkOccupied1 = 10;
		x += 10;
	} else if(x >= 1000 && y < 1000) {
		forkOccupied1 = 0;
		y += 10;
	} else if(x >= 1000 && y >= 1000 && r < PI) {
		r += PI/180.0f;
		forkHeight += 2;
	} else {
		forkHeight = 0;
		r = -PI;
		x -= 7;
		y -= 7;
	}

	camPosition.at<float>(0,0) = x;
	camPosition.at<float>(0,1) = y;
	camPosition.at<float>(0,2) = 0;
	camRotation.at<float>(0,0) = 0;
	camRotation.at<float>(0,1) = 0;
	camRotation.at<float>(0,2) = r;

	/*camPosition.at<float>(0,0) = rand()%10000-5000;
	camPosition.at<float>(0,1) = rand()%10000-5000;
	camPosition.at<float>(0,2) = rand()%10000-5000;
	camRotation.at<float>(0,0) = ((double)rand()/RAND_MAX)*2*PI-PI;
	camRotation.at<float>(0,1) = ((double)rand()/RAND_MAX)*2*PI-PI;
	camRotation.at<float>(0,2) = ((double)rand()/RAND_MAX)*2*PI-PI;*/

	// remove translation coordinates which are not configured
	if((transCoords_ & 1) == 0)
		camPosition.at<float>(0,0) = 0; // remove x position
	if((transCoords_ & 2) == 0)
		camPosition.at<float>(0,1) = 0; // remove y position
	if((transCoords_ & 4) == 0)
		camPosition.at<float>(0,2) = 0; // remove z position

	// remove rotation coordinates which are not configured
	if((rotCoords_ & 1) == 0)
		camRotation.at<float>(0,0) = 0; // remove x rotation
	if((rotCoords_ & 2) == 0)
		camRotation.at<float>(0,1) = 0; // remove y rotation
	if((rotCoords_ & 4) == 0)
		camRotation.at<float>(0,2) = 0; // remove z rotation

	timestamp = currentTimeOfDayMilliseconds();
	//std::cout << "timestamp=" << timestamp << " t=(" << camPosition.at<float>(0,0) << "," << camPosition.at<float>(0,1) << ";" << camPosition.at<float>(0,2) << ") r=(" << 180.0f*camRotation.at<float>(0,0)/PI << "," << 180.0f*camRotation.at<float>(0,1)/PI << ";" << 180.0f*camRotation.at<float>(0,2)/PI << ")" << std::endl;
}

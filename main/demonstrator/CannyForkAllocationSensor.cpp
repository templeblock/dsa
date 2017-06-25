/*
 * CannyForkAllocationSensor.cpp
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#include "CannyForkAllocationSensor.h"

dsa::CannyForkAllocationSensor::CannyForkAllocationSensor(VideoHub* dataHub, const bool isColor) : dsa::Sensorfunction(SensorType::ForkOccupied) {
	isColor_ = isColor;
	hub_ = dataHub;
	state_ = unknown;

	init();
}

dsa::CannyForkAllocationSensor::~CannyForkAllocationSensor() {
}

void dsa::CannyForkAllocationSensor::init() {
	GrabImageFilter		*filter05 = new GrabImageFilter(*hub_);
	CvtColorFilter		*filter0 = new CvtColorFilter(CV_RGB2GRAY);	// convert to grayscale
	filter0->setActive(isColor_);
	SubtractMeanFilter 	*filter1 = new SubtractMeanFilter();
	ForkChangeFilter 	*filter2 = new ForkChangeFilter("forkchange");
	ImageOutputFilter	*filter3 = new ImageOutputFilter("Gabelansicht");	// show video
	WaitFilter			*filter4 = new WaitFilter(15.0f);	// wait assuming 15fps

	// show video again, this time with the mask and only inside the roi
	filter0->src = &(filter05->dst);
	filter1->src = &(filter0->dst);
	filter2->src = &(filter1->dst);
	filter3->src = &(filter2->dst);
	filter4->src = &(filter3->dst);

	// list of the filters for phase 3
	std::list<Filter*> *filterList = new std::list<Filter*>();
	// add filters to list
	filterList->push_back(filter05);
	filterList->push_back(filter0);
	filterList->push_back(filter1);
	filterList->push_back(filter2);
	filterList->push_back(filter3);
	filterList->push_back(filter4);

	// create filterchain for phase 3
	filterChain_ = new FilterChain(filterList, true);
}

bool dsa::CannyForkAllocationSensor::calibrate() {
	return(true);
}

void dsa::CannyForkAllocationSensor::run() {
	for(;;) {	// loop forever
		filterChain_->apply();	// apply all filters in chain
		if(cv::waitKey(1) == 'q')
			break;
	}
}

/**
 * Returns the current state of the sensor function
 *
 * @return the current state
 */
dsa::ForkState dsa::CannyForkAllocationSensor::getState() {
	return(state_);
}

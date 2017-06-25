/*
 * ForkLearner.cpp
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#include "ForkLearner.h"

dsa::ForkLearner::ForkLearner(DataHub* dataHub, ForkAllocationSensorInitializer* fasi) : dsa::Sensorfunction(SensorType::ForkOccupied) {
	hub_ = dataHub;
	fasi_ = fasi;
	rois_ = &(fasi_->getROIs());
	forks_ = &(fasi_->getForks());
	forkMasks_ = &(fasi_->getMasks());
	forkReferences_ = &(fasi_->getReferences());
	maskedPixelsPerLine_ = &(fasi_->getMaskedPixelsPerLine());

	isReady_ = false;

	init();
}

dsa::ForkLearner::~ForkLearner() {
}

void dsa::ForkLearner::init() {

	isReady_ = true;
}

bool dsa::ForkLearner::calibrate() {
	return(true);
}

void dsa::ForkLearner::run() {

}

bool dsa::ForkLearner::isReady() {
	return(isReady_);
}

void dsa::ForkLearner::reset() {
}

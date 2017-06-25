/*
 * ForkAllocationEvent.cpp
 *
 *  Created on: 14.01.2013
 *      Author: jung
 */

#include "ForkAllocationEvent.h"
#include <iostream>
#include <cmath>

dsa::ForkAllocationEvent::ForkAllocationEvent(const int histSize) {
	state_ = unknown;
	lostFor_ = 0;
	lostThreshold_ = 5.0;
	windowSize_ = 30;
	lostRatio_ = 0.75;

	meanMeanBuffer_ = MeanQueueBuffer<double>(histSize);
	meanMeanBuffer_.write(lostThreshold_);	// initial static threshold, has to be a safe value (lower as the others because of different detection strategy (big => event)
	diffMeanBuffer_ = MeanQueueBuffer<double>(8);
	diffMeanBuffer_.write(lostThreshold_);	// initial static threshold, has to be a safe value (lower as the others because of different detection strategy (big => event)
}

dsa::ForkAllocationEvent::~ForkAllocationEvent() {
}

dsa::ForkState dsa::ForkAllocationEvent::getState() {
	return(state_);
}

dsa::ForkState dsa::ForkAllocationEvent::calcState(std::vector<double> &binMean, std::vector<double> &binDiff) {
	int nbins = binMean.size();

	// determine how many bins are over threshold in each third of the fork
	double meanThreshold = std::max(2*meanMeanBuffer_.getMean(), 20.0); // threshold should not be < 20; // holds dynamic threshold (total)
	double diffThreshold = 10.0; // threshold should not be < 10; // holds dynamic threshold (total)
	int aboveMeanThreshold = 0;
	double meanMean = 0.0;
	double diffMean = 0.0;
	for(size_t i = 0; i < nbins; i++) {
		if(binMean[i] > meanThreshold)	// bin j in third i is over threshold
			aboveMeanThreshold++;
		else
			meanMean += binMean[i];
		diffMean += binDiff[i];
	}
	diffMean /= nbins;

	const double meanOccupiedRatio = 0.60;	// when idle it needs to be above this ratio to count as occupied
	const double meanIdleRatio = 0.30;	// when occupied it needs to be below this ratio to count as idle
	const double diffOccupiedRatio = 0.70;	// when idle it needs to be above this ratio to count as occupied
	const double diffIdleRatio = 0.50;	// when occupied it needs to be below this ratio to count as idle

	double meanRatio = aboveMeanThreshold/(double)nbins;

	// update mean value for thresholds
	if(meanRatio <= meanIdleRatio)
		meanMeanBuffer_.write(meanMean/(nbins-aboveMeanThreshold));

	// we are lost after over 3/4 of windowSize frames are over lostThreshold_
	/*if(state_ != lost) {
		double lostRatio = (double)lostFor_/(double)windowSize_;
		if(lostRatio > 0.75) {
			prevState_ = state_;
			state_ = lost;
			//std::cout << frame_ << ", " << id_ << ": state changed to lost" << std::endl;
		} else {
			if(mean > lostThreshold_) {
				if(lostFor_ < windowSize_)
					lostFor_++;
			} else {
				lostFor_--;
			}
		}
	}*/

	diffMeanBuffer_.write(diffMean);
	switch(state_) {
	case idle:
		if(meanRatio >= meanOccupiedRatio && diffMeanBuffer_.getMean() >= diffThreshold*diffOccupiedRatio)
			state_ = occupied;
		break;
	case occupied:
		if(meanRatio <= meanIdleRatio && diffMeanBuffer_.getMean() < diffThreshold*diffIdleRatio)
			state_ = idle;
		break;
	case unknown:
		if(meanRatio <= meanIdleRatio && diffMeanBuffer_.getMean() < diffThreshold*diffIdleRatio)
			state_ = idle;
		else if(meanRatio >= meanOccupiedRatio && diffMeanBuffer_.getMean() >= diffThreshold*diffOccupiedRatio)
			state_ = occupied;
		break;
	case lost:
		state_ = lost;
		break;
	}

	return(state_);
}

std::vector<double> dsa::ForkAllocationEvent::getMean() {
	std::vector<double> vec = std::vector<double>();
	vec.push_back(meanMeanBuffer_.getMean());
	vec.push_back(diffMeanBuffer_.getMean());

	return(vec);
}

void dsa::ForkAllocationEvent::reset() {
	state_ = unknown;
	lostFor_ = 0;
	meanMeanBuffer_.free();
	meanMeanBuffer_.write(lostThreshold_);	// same as above
	diffMeanBuffer_.free();
	diffMeanBuffer_.write(lostThreshold_);	// same as above
}

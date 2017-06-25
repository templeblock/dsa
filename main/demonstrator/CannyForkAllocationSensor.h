/*
 * CannyForkAllocationSensor.h
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#ifndef CANNYFORKALLOCATIONSENSOR_H_
#define CANNYFORKALLOCATIONSENSOR_H_

#include <stdio.h>
#include "Sensorfunction.h"
#include "ForkAllocationSensorInitializer.h"
#include "ForkObserver.h"
#include <filter/ForkChangeFilter.h>
#include <filter/AdaptiveThresholdFilter.h>
#include <filter/UnsharpMaskFilter.h>
#include <filter/ForkPerimeterFinderFilter.h>
#include <filter/DilateFilter.h>
#include <filter/SobelFilter.h>
#include <filter/GrabImageFilter.h>
#include <sensordata/VideoHub.h>
#include "generic/helper.h"

namespace dsa {

class CannyForkAllocationSensor : public Sensorfunction {

	bool isColor_;
	VideoHub* hub_;
	ForkState state_;	// joint state of both forks

	FilterChain *filterChain_;

	void init();

public:
	CannyForkAllocationSensor(VideoHub* dataHub, const bool isColor=true);
	virtual ~CannyForkAllocationSensor();

	bool calibrate();
	void run();

	ForkState getState();
};

}
#endif /* CANNYFORKALLOCATIONSENSOR_H_ */

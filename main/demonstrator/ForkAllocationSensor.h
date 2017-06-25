/*
 * ForkAllocationSensor.h
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#ifndef FORKALLOCATIONSENSOR_H_
#define FORKALLOCATIONSENSOR_H_

#include <stdio.h>
//#include "ForkInfo.h"
#include "Sensorfunction.h"
#include "ForkAllocationSensorInitializer.h"
#include "ForkObserver.h"
#include <filter/helper/WaitForInputWorker.h>
#include <eval/EvaluationLogger.h>
#include <sensordata/VideoHub.h>

namespace dsa {

class ForkAllocationSensor : public Sensorfunction {

	VideoHub* hub_;
	ForkState state_;	// joint state of both forks

	ForkAllocationSensorInitializer* fasi_;
	ForkObserver* fo_;

public:
	ForkAllocationSensor(VideoHub* dataHub);
	virtual ~ForkAllocationSensor();

	bool calibrate();
	void run();

	ForkState getState();
};

}
#endif /* FORKALLOCATIONSENSOR_H_ */

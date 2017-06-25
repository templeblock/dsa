/*
 * ForkAllocationSensor.h
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#ifndef FORKOBSERVER_H_
#define FORKOBSERVER_H_

#include "Sensorfunction.h"
#include "ForkInfo.h"
#include "ForkAllocationSensorInitializer.h"
#include <sensordata/VideoHub.h>
#include <filter/FilterChain.h>
#include <filter/CustomFilter.h>
#include <filter/CvtColorFilter.h>
#include <filter/DrawPolylineFilter.h>
#include <filter/EqualizeHistFilter.h>
#include <filter/SubtractMeanFilter.h>
#include <filter/SubtractReferenceFilter.h>
#include <filter/ImageOutputFilter.h>
#include <filter/ResizeFilter.h>
#include <filter/PutTextFilter.h>
#include <filter/VideoWriterFilter.h>
#include <filter/MaskFilter.h>
#include <filter/RoiFilter.h>
#include <filter/WaitFilter.h>
#include <filter/UncompressedVideoWriterFilter.h>
#include <filter/CamshiftFilter.h>

namespace dsa {

class ForkObserver : public Sensorfunction {
	bool isReady_;

	ForkInfo* finfo_;
	VideoHub* hub_;
	ForkAllocationSensorInitializer* fasi_;
	FilterChain *filterChain_;

	void init();

public:
	ForkObserver(VideoHub* dataHub, ForkAllocationSensorInitializer* fasi);
	virtual ~ForkObserver();

	bool calibrate();
	void run();

	bool isReady();
	void reset();

	void setInitializer(ForkAllocationSensorInitializer* fasi);
	ForkInfo* getForkInfo();
};

}
#endif /* FORKOBSERVER_H_ */

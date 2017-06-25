/*
 * ForkAllocationSensor.cpp
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#include "ForkAllocationSensor.h"

dsa::ForkAllocationSensor::ForkAllocationSensor(VideoHub* dataHub) : dsa::Sensorfunction(SensorType::ForkOccupied) {
	hub_ = dataHub;
	fasi_ = 0;
	fo_ = 0;
	state_ = unknown;
}

dsa::ForkAllocationSensor::~ForkAllocationSensor() {
	delete fasi_;
	delete fo_;
}

bool dsa::ForkAllocationSensor::calibrate() {
	return(true);
}

void dsa::ForkAllocationSensor::run() {
	std::string configFilename = "forkAllocationConfig.xml";
	fasi_ = new ForkAllocationSensorInitializer(hub_, configFilename);

	//fasi_ = new ForkAllocationSensorInitializer(configFilename);
	// Initialize first
	while(!fasi_->isReady())
		fasi_->run();

	fo_ = new ForkObserver(hub_, fasi_);
	ForkAllocationSensorInitializer* fasiNew = NULL; // to update fork position

	WaitForInputWorker *waitForInputWorker = new WaitForInputWorker();
	boost::thread listenForInputThread = boost::thread(&WaitForInputWorker::run, &(*waitForInputWorker));
	// create logger
	std::string ausgabe = std::string("ausgabe_fa.csv");
	EvaluationLogger logger(ausgabe, waitForInputWorker);

	bool stop = false;
	while(!stop) {	// loop forever
		fo_->run();

		// Gabel belegt status für Weiterverarbeitung
		//state_ = fo_->getForkInfo()->getState();

		// do someting with the results
		int now = currentTimeOfDayMilliseconds();
		logger.log(fo_->getForkInfo());	// log it

		// quit on user input
		stop = waitForInputWorker->isFinished();
		if(cv::waitKey(1) == 'q')
			break;

		/*if(state_ == unknown) {  // we might lose the forks, so prepare a new ForkAllocationSensorInitializer just in case
			if(fasiNew == NULL)
				fasiNew = new ForkAllocationSensorInitializer(isColor_);
			fasiNew->determineBinaryImage(&((ImageSensorData*)hub_->getData(0,true))->getImage());
		} else if(state_ == lost) {	// when lost apply => use fasiNew
			bool reinitialize = false;
			if(fasiNew != NULL) { // we jumped to lost directly (nothing we can do to speed things up)
				std::remove(configFilename.c_str()); // delete old config file as it is no longer valid
				fasiNew->setConfig(configFilename);
				fasiNew->run();

				if(!fasi_->isReady()) {	// the created ForkAllocationSensorInitializer is worthless => throw away and start over
					delete fasiNew;
					reinitialize = true;
				}
			} else { // directly jumped to lost => no choice but to do the initial detection
				reinitialize = true;
			}
			if(reinitialize)  {
				std::remove(configFilename.c_str()); // delete old config file as it is no longer valid
				fasiNew = new ForkAllocationSensorInitializer(hub_, configFilename, isColor_);	// reinitialize
				while(!fasiNew->isReady())
					fasiNew->run();
			}

			fo_->setInitializer(fasiNew);	// update ForkObserver with the new initializer
			delete fasi_;
			fasi_ = fasiNew;
		} else {	// we are in good state, so we can remove any newly created ForkAllocationSensorInitializer
			if(fasiNew != NULL) {
				delete fasiNew;
				fasiNew = NULL;
			}
		}*/
	}

	// call join
	listenForInputThread.join();
	// delete all the stuff
	delete waitForInputWorker;
}

/**
 * Returns the current state of the sensor function
 *
 * @return the current state
 */
dsa::ForkState dsa::ForkAllocationSensor::getState() {
	return(state_);
}

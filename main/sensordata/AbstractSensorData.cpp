/**
 * SensorData.cpp
 *
 * Abstract sensordata model. This class serves as a basic datastructure for the timestamps at which
 * sensordata was obtained. Two basic values are necessary:
 *
 * grabTimestamp, the timestamp denoting the moment the data was grabbed.
 * dataTimestamp, the timestamp showing when the measuring occured.
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#include "AbstractSensorData.h"

/**
 * Constructor. Initializes both timestamps with the (same) current clock() value.
 *
 */
dsa::AbstractSensorData::AbstractSensorData() {
	clock_t timestamp = clock();
	init(timestamp, timestamp);
}

/**
 * Constructor. Initializes both timestamps with the (same) value from the parameter.
 *
 * @param grabTimestamp
 */
dsa::AbstractSensorData::AbstractSensorData(const clock_t grabTimestamp) {
	init(grabTimestamp, grabTimestamp);
}

/**
 * Constructor. Initializes both timestamps with the value from the parameters.
 *
 * @param grabTimestamp
 * @param dataTimestamp
 */
dsa::AbstractSensorData::AbstractSensorData(const clock_t grabTimestamp, const clock_t dataTimestamp) {
	init(grabTimestamp, dataTimestamp);
}

/**
 * Copy constructor. Creates an identical clone of an instance of this class.
 *
 * @param instance to clone
 *
 */
dsa::AbstractSensorData::AbstractSensorData(const AbstractSensorData& copy) {
	copy_ = true;
	grabTimestamp_ = copy.grabTimestamp_;
	dataTimestamp_ = copy.dataTimestamp_;
}

/**
 * Destructor.
 */
dsa::AbstractSensorData::~AbstractSensorData() {
}

/**
 * Initialize attributes shared among all ctors
 *
 * @param grabTimestamp
 * @param dataTimestamp
 */
void dsa::AbstractSensorData::init(const clock_t grabTimestamp, const clock_t dataTimestamp) {
	copy_ = false;
	grabTimestamp_ = grabTimestamp;
	dataTimestamp_ = dataTimestamp;
}

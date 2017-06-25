/**
 * DataHub.cpp
 *
 * A very generic hub base-class for any sensordata one would like served.
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#include "DataHub.h"

/**
 * Constructor. Initialize Hub.
 */
dsa::DataHub::DataHub() {
	lastGrabbed_ = 0;
}

/**
 * Destructor.
 */
dsa::DataHub::~DataHub() {
}

/**
 * Perform generic tasks necessary during grabbing. Updates lastGrabbed timestamp.
 */
void dsa::DataHub::grabData() {
	lastGrabbed_ = currentTimeOfDayMilliseconds();
}

/**
 * Determine whether grabbing is necessary of the currently held frame is acceptable.
 *
 * @param clock_t the maximum age allowed
 * @return true if we need to grab a new frame, false otherwise.
 */
bool dsa::DataHub::needsGrabbing(const clock_t clock_t) {
	return(lastGrabbed_ > 0 && clock_t > lastGrabbed_);
}

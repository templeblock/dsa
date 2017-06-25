/*
 * Sensorfunction.cpp
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#include "Sensorfunction.h"

dsa::Sensorfunction::Sensorfunction(SensorType type) {
	type_ = type;
}

dsa::Sensorfunction::~Sensorfunction() {
}

dsa::SensorType dsa::Sensorfunction::getType() {
	return(type_);
}

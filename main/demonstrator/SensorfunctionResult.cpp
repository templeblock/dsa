/*
 * SensorfunctionResult.cpp
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#include "SensorfunctionResult.h"

dsa::SensorfunctionResult::SensorfunctionResult(SensorType type) {
	type_ = type;
	isValid = false;
	timestamp = 0;
}

dsa::SensorfunctionResult::SensorfunctionResult(const SensorfunctionResult& rhs) {
	type_ = rhs.type_;
	isValid = rhs.isValid;
	timestamp = rhs.timestamp;
}

dsa::SensorfunctionResult::~SensorfunctionResult() {

}

dsa::SensorType dsa::SensorfunctionResult::getType() {
	return(type_);
}

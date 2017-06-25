/*
 * ForkHeightResult.cpp
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#include "ForkHeightResult.h"

dsa::ForkHeightResult::ForkHeightResult() : SensorfunctionResult(SensorType::ForkHeight) {
	h = 0.f;
	r = 0.f;
	endX = 0.f;
	endZ = 0.f;
}

dsa::ForkHeightResult::ForkHeightResult(const ForkHeightResult& rhs) : SensorfunctionResult(rhs){
	h = rhs.h;
	r = rhs.r;
	endX = rhs.endX;
	endZ = rhs.endZ;
}

dsa::ForkHeightResult::~ForkHeightResult() {
}

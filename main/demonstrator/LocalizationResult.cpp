/*
 * LocalizationResult.cpp
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#include "LocalizationResult.h"

dsa::LocalizationResult::LocalizationResult() : SensorfunctionResult(SensorType::Localization) {
	t[0] = 0.f,	t[1] = 0.f, t[2] = 0.f;
	r[0] = 0.f, r[1] = 0.f, r[2] = 0.f;
}

dsa::LocalizationResult::LocalizationResult(const LocalizationResult& rhs) : SensorfunctionResult(rhs){
	t[0] = rhs.t[0], t[1] = rhs.t[1], t[2] = rhs.t[2];
	r[0] = rhs.r[0], r[1] = rhs.r[1], r[2] = rhs.r[2];
}

dsa::LocalizationResult::~LocalizationResult() {
}

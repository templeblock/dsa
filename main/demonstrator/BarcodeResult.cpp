/*
 * BarcodeResult.cpp
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#include "BarcodeResult.h"

dsa::BarcodeResult::BarcodeResult() : SensorfunctionResult(SensorType::CodeReader) {
	std::vector<BarcodeEvent> barcodeEvents = std::vector<BarcodeEvent>();
}

dsa::BarcodeResult::BarcodeResult(const BarcodeResult& rhs) : SensorfunctionResult(rhs){
	barcodeEvents = std::vector<BarcodeEvent>(rhs.barcodeEvents);
}

dsa::BarcodeResult::~BarcodeResult() {
}

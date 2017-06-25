/*
 * BarcodeResult.h
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#ifndef BARCODERESULT_H_
#define BARCODERESULT_H_

#include <vector>
#include "SensorfunctionResult.h"

namespace dsa {

struct BarcodeResult : public dsa::SensorfunctionResult {

public:

	std::vector<BarcodeEvent> barcodeEvents;

	BarcodeResult();
	BarcodeResult(const BarcodeResult& rhs);
	virtual ~BarcodeResult();
};

} /* namespace dsa */

#endif /* BARCODERESULT_H_ */

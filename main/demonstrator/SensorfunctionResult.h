/*
 * SensorfunctionResult.h
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#ifndef SENSORFUNCTIONRESULT_H_
#define SENSORFUNCTIONRESULT_H_

#include <generic/types.h>

namespace dsa {

class SensorfunctionResult {
	SensorType type_;
public:
	bool isValid;
	int timestamp;

	SensorfunctionResult(SensorType type);
	SensorfunctionResult(const SensorfunctionResult& rhs);
	~SensorfunctionResult();

	SensorType getType();
};

} /* namespace dsa */

#endif /* SENSORFUNCTIONRESULT_H_ */

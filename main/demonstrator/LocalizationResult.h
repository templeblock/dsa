/*
 * LocalizationResult.h
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#ifndef LOCALIZATIONRESULT_H_
#define LOCALIZATIONRESULT_H_

#include "SensorfunctionResult.h"

namespace dsa {

struct LocalizationResult: public dsa::SensorfunctionResult {

public:
	float t[3];
	float r[3];

	LocalizationResult();
	LocalizationResult(const LocalizationResult& rhs);
	virtual ~LocalizationResult();
};

} /* namespace dsa */

#endif /* LOCALIZATIONRESULT_H_ */

/*
 * ForkHeightResult.h
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#ifndef FORKHEIGHTRESULT_H_
#define FORKHEIGHTRESULT_H_

#include "SensorfunctionResult.h"

namespace dsa {

struct ForkHeightResult : public dsa::SensorfunctionResult {

public:
	float h;
	float r;
	float endX;
	float endZ;

	ForkHeightResult();
	ForkHeightResult(const ForkHeightResult& rhs);
	virtual ~ForkHeightResult();
};

} /* namespace dsa */

#endif /* FORKHEIGHTRESULT_H_ */

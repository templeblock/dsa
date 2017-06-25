/*
 * WaitFilter.h
 *
 *	This filter does nothing but wait to sync to a specific framerate, where possible. Since we cannot travel back
 *	in time, we can only sync to the future (i.e. wait until sync is restored).
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef WAITFILTER_H_
#define WAITFILTER_H_

#include <boost/thread/thread.hpp>
#include <generic/types.h>
#include "EvaluationFilter.h"

namespace dsa {

class WaitFilter : public EvaluationFilter {

	bool waitForInput_;
	bool anyKey_;
	char key_;
	clock_t last_;
	int msPerFrame_;

	void evaluate();

public:

	WaitFilter();
	WaitFilter(const char key);
	WaitFilter(const float fps);
	virtual ~WaitFilter();

	bool isReady() const;
};

}
#endif /* WAITFILTER_H_ */

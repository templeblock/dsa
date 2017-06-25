/*
 * ForkAllocationEvent.h
 *
 *  Created on: 14.01.2013
 *      Author: jung
 */

#ifndef FORKALLOCATIONEVENT_H_
#define FORKALLOCATIONEVENT_H_

#include <vector>
#include <generic/types.h>
#include <generic/MeanQueueBuffer.h>

namespace dsa {

class ForkAllocationEvent {

	int lostFor_;
	int windowSize_;
	double lostThreshold_;
	double lostRatio_;

	ForkState state_;	// saves the current allocation status of each fork
	MeanQueueBuffer<double> meanMeanBuffer_;
	MeanQueueBuffer<double> diffMeanBuffer_;

public:
	ForkAllocationEvent(const int histSize=1000);
	virtual ~ForkAllocationEvent();

	ForkState getState();
	ForkState calcState(std::vector<double> &binMean, std::vector<double> &binDiff);
	std::vector<double> getMean();
	void reset();
};

}
#endif /* FORKALLOCATIONEVENT_H_ */

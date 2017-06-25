/*
 * ForkLearner.h
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#ifndef FORKLEARNER_H_
#define FORKLEARNER_H_

#include "Sensorfunction.h"
#include "ForkAllocationSensorInitializer.h"
#include <opencv2/opencv.hpp>
#include <generic/helper.h>
#include <sensordata/VideoHub.h>

namespace dsa {

class ForkLearner : public Sensorfunction {
	bool isReady_;
	DataHub* hub_;
	ForkAllocationSensorInitializer* fasi_;

	FilterChain *filterChain_;
	std::vector<cv::Rect> *rois_;
	std::vector<std::vector<cv::Point> > *forks_;
	std::vector<cv::Mat> *forkMasks_;
	std::vector<cv::Mat> *forkReferences_;
	std::vector<std::vector<size_t> *> *maskedPixelsPerLine_;

	void init();

public:
	ForkLearner(DataHub* dataHub, ForkAllocationSensorInitializer* fasi);
	virtual ~ForkLearner();

	bool calibrate();
	void run();

	bool isReady();
	void reset();

};

}
#endif /* FORKLEARNER_H_ */

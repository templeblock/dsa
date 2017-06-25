/*
 * AutoSensorFilter.h
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef AUTOSENSORFILTER_H_
#define AUTOSENSORFILTER_H_

#include "EvaluationFilter.h"
#include <opencv2/opencv.hpp>
#include <sensordata/autoadjust/AutoSensorAlgorithm.h>

namespace dsa {

class AutoSensorFilter : public EvaluationFilter {

	bool usemask_;
	AutoSensorAlgorithm* autosensorAlgorithm_;
	cv::Mat mask_;

	void init();
	void evaluate();

public:

	bool* usemask;
	cv::Mat* mask;

	AutoSensorFilter(AutoSensorAlgorithm* const autosensorAlgorithm, cv::Mat mask=cv::Mat());
	virtual ~AutoSensorFilter();
};

}
#endif /* AUTOSENSORFILTER_H_ */

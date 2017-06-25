/*
 * StatModelFilter.h
 *
 * 	Base class for statistical models
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef STATMODELFILTER_H_
#define STATMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "EvaluationFilter.h"

namespace dsa {

class StatModelFilter : public EvaluationFilter {

protected:

	bool autoLearn_;
	float autoLearnThreshold_;
	MLType type_;

	const char* filename_;	// file
	const char* name_;	// model within file

	virtual void init();
	virtual void evaluate();
	virtual void predict() = 0;

	std::vector<std::vector<float> > lValues_;
	std::vector<GenericMLResult> lClasses_;

public:

	virtual bool isReady();
	virtual void preprocessing();

	cv::Mat *descriptorValues;
	std::vector<GenericMLResult> results;

	StatModelFilter(const char* filename, const char* name=0, bool autoLearn=false, float autoLearnThreshold=0.98f);
	virtual ~StatModelFilter();
};

}
#endif /* STATMODELFILTER_H_ */

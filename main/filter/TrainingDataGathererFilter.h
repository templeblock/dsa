/*
 * TrainingDataGathererFilter.h
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef TRAININGDATAGATHERERFILTER_H_
#define TRAININGDATAGATHERERFILTER_H_

#include <fstream>
#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include <generic/TrainingDataManager.h>
#include "EvaluationFilter.h"

namespace dsa {

class TrainingDataGathererFilter : public EvaluationFilter {
	bool takeNext_;
	int everyNth_;
	UINT32 run_;
	TrainingDataManager<std::vector<float> > *tdm_;

	void init();
	void evaluate();

public:

	TrainingDataGathererFilter(const std::string &filename, const UINT32 maxElements=5000, const UINT32 everyNth=1000);
	virtual ~TrainingDataGathererFilter();
};

}
#endif /* TRAININGDATAGATHERERFILTER_H_ */

/*
 * BayesModelFilter.h
 *
 * 	Normal bayes model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef BAYESMODELFILTER_H_
#define BAYESMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "StatModelFilter.h"

namespace dsa {

class BayesModelFilter : public StatModelFilter {

	CvNormalBayesClassifier bayes_;

	void predict();

public:

	BayesModelFilter(const char* filename, const char* name=0);
	virtual ~BayesModelFilter();
};

}
#endif /* BAYESMODELFILTER_H_ */

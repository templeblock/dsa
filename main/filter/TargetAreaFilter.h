/*
 * TargetAreaFilter.h
 *
 *	Determines target areas from a vector of values
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef TARGETAREAFILTER_H_
#define TARGETAREAFILTER_H_

#include <generic/types.h>
#include "EvaluationFilter.h"
#include <generic/Predictor.h>
#include <generic/MathVector.h>

namespace dsa {

class TargetAreaFilter : public EvaluationFilter {

	MathVector<int> areas_;
	void evaluate();

public:

	MathVector<int>* areas;
	std::vector<Circle> figures;

	void preprocessing();

	TargetAreaFilter(MathVector<int> mathVector=MathVector<int>());
	virtual ~TargetAreaFilter();
};

}
#endif /* TARGETAREAFILTER_H_ */

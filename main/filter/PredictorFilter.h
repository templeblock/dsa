/*
 * PredictorFilter.h
 *
 *	Predicts various values based on past values.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef PREDICTORFILTER_H_
#define PREDICTORFILTER_H_

#include <generic/types.h>
#include "EvaluationFilter.h"
#include <generic/Predictor.h>

namespace dsa {

template <class T> class PredictorFilter : public EvaluationFilter {

	Predictor<T> predictor_;
	void evaluate() {
		prediction = predictor_.next();
	}

public:

	T prediction;
	PredictorFilter(Predictor<T> predictor) {
		predictor_ = predictor;
	}
	virtual ~PredictorFilter();
};

}
#endif /* PREDICTORFILTER_H_ */

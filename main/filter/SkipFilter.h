/*
 * SkipFilter.h
 *
 * 	Only pass-through n out of m frames
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef SKIPFILTER_H_
#define SKIPFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class SkipFilter : public EvaluationFilter {

	UINT32 n_;
	UINT32 m_;
	UINT32 run_;
	UINT32 lastExec_;
	double div_;

	void init();
	void evaluate();

public:

	SkipFilter(UINT32 m, UINT32 n=1);
	virtual ~SkipFilter();
};

}
#endif /* SKIPFILTER_H_ */

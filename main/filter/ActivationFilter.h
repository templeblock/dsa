/*
 * ActivationFilter.h
 *
 * 	This simple filter activates and deactivates the succeeding filter depending on an input variable. It comes as a separate filter to avoid
 * 	the specific filters that are to be deactivated or trying to deactivate a filter to have additional code for this purpose.
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef ACTIVATIOINFILTER_H_
#define ACTIVATIOINFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class ActivationFilter : public EvaluationFilter {

	bool status_;

	void init();
	void evaluate();

public:

	bool* status;

	ActivationFilter(const bool initialStatus=true);
	virtual ~ActivationFilter();
};

}
#endif /* ACTIVATIOINFILTER_H_ */

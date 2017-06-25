/*
 * SimulateTagsFilter.h
 *
 *	This filter simulates tag information for testing purposes.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef SIMULATETAGSFILTER_H_
#define SIMULATETAGSFILTER_H_

#include <cmath>
#include <vector>
#include <generic/helper.h>
#include "EvaluationFilter.h"

namespace dsa {

class SimulateTagsFilter : public EvaluationFilter {

	int maxTags_;
	int maxTagLength_;

	void init();
	void evaluate();

public:

	clock_t timestamp;
	std::vector<int> types;
	std::vector<std::string> contents;

	SimulateTagsFilter(int maxTags, int maxTagLength);
	virtual ~SimulateTagsFilter();

	bool isReady() const;
	void preprocessing();
};

}
#endif /* SIMULATETAGSFILTER_H_ */

/*
 * TagsToDatagramFilter.cpp
 *
 * Export tag information via a datagram
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef TAGSTODATAGRAMFILTER_H_
#define TAGSTODATAGRAMFILTER_H_

#include <vector>
#include <generic/helper.h>
#include "EvaluationFilter.h"

namespace dsa {

class TagsToDatagramFilter : public EvaluationFilter {

	int packetType_;
	int vehicleID_;
	clock_t timestamp_;

	std::vector<int> types_;
	std::vector<std::string> contents_;

	void init();
	void preprocessing();
	void evaluate();

public:

	clock_t *timestamp;
	std::vector<int> *types;
	std::vector<std::string> *contents;
	std::vector<char> data;

	TagsToDatagramFilter(int vehicleID);
	virtual ~TagsToDatagramFilter();
};

}
#endif /* TAGSTODATAGRAMFILTER_H_ */

/*
 * FilterChain.h
 *
 * Represents a set of filters that are executed in sequence. Since a filter chain is also a Filter itself,
 * several FilterChains can be merged together inside another filter chain, hence connecting modules of different
 * abstraction levels.
 *
 *  Created on: 27.06.2012
 *      Author: jung
 */

#ifndef FILTERCHAIN_H_
#define FILTERCHAIN_H_

#include <list>
#include "Filter.h"

namespace dsa {

class FilterChain : public Filter {

	bool standalone_;
	bool deleteFilters_;
	std::list<Filter*> *chain_;

public:

	FilterChain(bool deleteFilters=false);
	FilterChain(std::list<Filter*> *chain, bool deleteFilters=false);
	virtual ~FilterChain();

	virtual void preprocessing();
	virtual void apply();
	virtual void bypass();
	virtual void postprocessing();

	virtual bool isReady() const;

	std::list<Filter*>* getChain();
	void setChain(std::list<Filter*> *v);
};

}
#endif /* FILTERCHAIN_H_ */

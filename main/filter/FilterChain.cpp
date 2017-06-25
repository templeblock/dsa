/*
 * FilterChain.cpp
 *
 * Represents a set of filters that are executed in sequence. Since a filter chain is also a Filter itself,
 * several FilterChains can be merged together inside another filter chain, hence connecting modules of different
 * abstraction levels.
 *
 *  Created on: 27.06.2012
 *      Author: jung
 */

#include "FilterChain.h"
#include <iostream>
/**
 * Constructor
 *
 * @param deleteFilters: iff true, delete the filters when chain is destroyed
 */
dsa::FilterChain::FilterChain(bool deleteFilters) : dsa::Filter() {
	chain_ = new std::list<Filter*>();
	standalone_ = true;
	deleteFilters_ = deleteFilters;
}

/**
 * Constructor
 *
 * @param chain: list of filters to run in the given sequence.
 * @param deleteFilters: iff true, delete the filters when chain is destroyed
 */
dsa::FilterChain::FilterChain(std::list<dsa::Filter*> *chain, bool deleteFilters) : dsa::Filter() {
	chain_ = chain;
	standalone_ = true;
	deleteFilters_ = deleteFilters;
}

/**
 * Destructor
 *
 * Destroy all filters in the chain if configured.
 */
dsa::FilterChain::~FilterChain() {
	if(deleteFilters_) {
		for(std::list<Filter*>::const_iterator c_it_ = chain_->begin(); c_it_ != chain_->end(); c_it_++)
			delete *c_it_;
		delete chain_;
	}
}

/**
 * Things to do before the filters are applied
 */
void dsa::FilterChain::preprocessing() {
	Filter* prev = 0;
	for(std::list<Filter*>::const_iterator c_it_ = chain_->begin(); c_it_ != chain_->end();) {
		(*c_it_)->setPredecessor(prev);
		prev = *c_it_;
		c_it_++;
		if(c_it_ != chain_->end())
			prev->setSuccessor(*c_it_);
	}
}

#include <iomanip>
/**
 * Apply all active filters in sequence.
 */
void dsa::FilterChain::apply() {
	preprocessing();
	int i = 0;
	clock_t cstart = clock();

	for(std::list<Filter*>::const_iterator c_it_ = chain_->begin(); c_it_ != chain_->end(); c_it_++) {
		clock_t fstart = clock();
		(*c_it_)->preprocessing();
		if((*c_it_)->isActive())
			(*c_it_)->apply();
		else
			(*c_it_)->bypass();
		(*c_it_)->postprocessing();

		clock_t fend = clock();
		std::cout<<std::fixed<<std::setprecision(2)<<(1000*((float)fend-fstart))/CLOCKS_PER_SEC<<" filter: "<< i<<std::endl;
		i++;
	}
	std::cout<<"\nFilterChain::apply "<<((float)clock()-cstart)/CLOCKS_PER_SEC<<std::endl;
	//std::cout<<"/"<<((float)clock()-cstart)<<std::endl;
}

/**
 * Bypass all filters.
 */
void dsa::FilterChain::bypass() {
	for(std::list<Filter*>::const_iterator c_it_ = chain_->begin(); c_it_ != chain_->end(); c_it_++) {
		(*c_it_)->preprocessing();
		(*c_it_)->bypass();
		(*c_it_)->postprocessing();
	}
}

/**
 * Things to perform after application of the chain
 */
void dsa::FilterChain::postprocessing() {
}

/**
 * Determine whether the chain is ready for execution. It is ready, if all filters in it are ready. This may need to be revised, so that only the first filter in chain needs to be ready. Otherwise a chain may never become ready.
 *
 * @return true if chain is ready.
 */
bool dsa::FilterChain::isReady() const {
	bool retval = true;
	for(std::list<Filter*>::const_iterator c_it_ = chain_->begin(); c_it_ != chain_->end(); c_it_++)
		if(!(*c_it_)->isReady())
			break;
	return(retval);
}

/**
 * Exposes the chain to perform changes
 *
 * @return the list of filters in the chain
 */
std::list<dsa::Filter*>* dsa::FilterChain::getChain() {
	return(chain_);
}

/**
 * Allows for setting a new chain.
 *
 * @param v
 */
void dsa::FilterChain::setChain(std::list<Filter*> *v) {
	chain_ = v;
}

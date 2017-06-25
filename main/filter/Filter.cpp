/*
 * Filter.cpp
 *
 *	Baseclass of ImageFilter and EvaluationFilter (and potentially other filter-related classes)
 *
 *  Created on: 27.06.2012
 *      Author: jung
 */

#include "Filter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::Filter::Filter() {
	predecessor_ = 0;
	successor_ = 0;
	isActive_ = true;
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::Filter::~Filter() {
}

dsa::Filter* dsa::Filter::getPredecessor() {
	return(predecessor_);
}

dsa::Filter* dsa::Filter::getSuccessor() {
	return(successor_);
}

void dsa::Filter::setPredecessor(dsa::Filter* const filter) {
	predecessor_ = filter;
}

void dsa::Filter::setSuccessor(dsa::Filter* const filter) {
	successor_ = filter;
}


bool dsa::Filter::isActive() const {
	return(isActive_);
}

void dsa::Filter::setActive(const bool active) {
	isActive_ = active;
}

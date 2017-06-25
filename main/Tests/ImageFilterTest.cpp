/*
 * ImageFilterTest.cpp
 *
 *  Created on: 27.09.2012
 *      Author: jung
 */

#include "ImageFilterTest.h"

/**
 * Constructor
 *
 * the filters in the filterList should already be properly connected to their corresponding predecessor and successor where necessary.
 *
 * @param filterList: List of properly connected filters to apply in the sequence they are configured in.
 * @param nruns: number of testruns to perform
 * @param logfileName: name of the logfile to write results to
 */
dsa::ImageFilterTest::ImageFilterTest(std::list<Filter*> *filterList, const UINT32 nruns, std::string logfileName) : TestRun(nruns, logfileName) {
	chain = FilterChain(filterList, true);
}

/**
 * Destructor
 */
dsa::ImageFilterTest::~ImageFilterTest() {
	// TODO Auto-generated destructor stub
}

void dsa::ImageFilterTest::logBefore(const UINT32 run) {
	std::string end = intToString(run).append(end2);
	std::string line = start1;
	line.append(end);
	log(line);
}

void dsa::ImageFilterTest::logAfter(const UINT32 run) {
	std::string end = intToString(run).append(end2);
	std::string line = end1;
	line.append(end);
	log(line);
}

/**
 * Apply all the filter chains and log start and end
 */
void dsa::ImageFilterTest::execute() {
	for(UINT32 i = 0; i < nruns_; i++) {	// loop for nruns_
		logBefore(i);

		chain.apply();	// apply all filters in chain
		update();		// update for next run

		logAfter(i);
	}
	logResults();
}

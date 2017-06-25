/*
 * ImageFilterTest.h
 *
 *  Created on: 27.09.2012
 *      Author: jung
 */

#ifndef IMAGEFILTERTEST_H_
#define IMAGEFILTERTEST_H_

#include <string>
#include <generic/helper.h>
#include <filter/FilterChain.h>
#include "TestRun.h"

static const std::string start1("--- start of run ");
static const std::string end1("--- end of run ");
static const std::string end2(" ---");

namespace dsa {

class ImageFilterTest : public TestRun {

protected:

	virtual void logBefore(const UINT32 run);
	virtual void logAfter(const UINT32 run);
	virtual void printResults() = 0;
	virtual void writeResults() = 0;
	virtual void update() = 0;

	FilterChain chain;

public:

	ImageFilterTest(std::list<Filter*> *chain, const UINT32 nruns=1, std::string logfileName="");
	virtual ~ImageFilterTest();

	void execute();

};

}
#endif /* IMAGEFILTERTEST_H_ */

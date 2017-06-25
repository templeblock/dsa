/*
 * CannyParameterTest.h
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef CANNYPARAMETERTEST_H_
#define CANNYPARAMETERTEST_H_

#include "ImageFilterTest.h"

namespace dsa {

class CannyParameterTest : public ImageFilterTest {
public:
	CannyParameterTest(std::list<Filter*> *chain, const UINT32 nruns=1, std::string logfileName="");
	virtual ~CannyParameterTest();
};

}
#endif /* CANNYPARAMETERTEST_H_ */

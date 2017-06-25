/*
 * ForkChangeFilter.h
 *
 * 	Filter background out from our view on the forks
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef FORKCHANGEFILTER_H_
#define FORKCHANGEFILTER_H_

#include <generic/helper.h>
#include "ImageBufferFilter.h"
#include <string>
#include <iostream>
#include <fstream>

namespace dsa {

class ForkChangeFilter : public ImageBufferFilter {

	UINT32 nruns_;

	std::string logfileName_;
	std::ofstream logfile_;

	void evaluate();

public:

	ForkChangeFilter(std::string logfileName);
	virtual ~ForkChangeFilter();

	void reset();
};

}
#endif /* FORKCHANGEFILTER_H_ */

/*
 * GBTModelFilter.h
 *
 * 	Gradient Boosted Tree model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef GBTMODELFILTER_H_
#define GBTMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "StatModelFilter.h"

namespace dsa {

class GBTModelFilter : public StatModelFilter {

	CvGBTrees gbt_;

	void predict();

public:

	GBTModelFilter(const char* filename, const char* name=0);
	virtual ~GBTModelFilter();
};

}
#endif /* GBTMODELFILTER_H_ */

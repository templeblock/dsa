/*
 * BOOSTModelFilter.h
 *
 * 	BOOSTING model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef BOOSTMODELFILTER_H_
#define BOOSTMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "StatModelFilter.h"

namespace dsa {

class BOOSTModelFilter : public StatModelFilter {

	CvBoost boost_;

	void predict();

public:

	BOOSTModelFilter(const char* filename, const char* name=0);
	virtual ~BOOSTModelFilter();
};

}
#endif /* BOOSTMODELFILTER_H_ */

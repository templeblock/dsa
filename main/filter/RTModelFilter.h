/*
 * RTModelFilter.h
 *
 * 	Random trees model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef RTMODELFILTER_H_
#define RTMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "StatModelFilter.h"

namespace dsa {

class RTModelFilter : public StatModelFilter {

	CvRTrees rts_;

	void predict();

public:

	RTModelFilter(const char* filename, const char* name=0);
	virtual ~RTModelFilter();
};

}
#endif /* RTMODELFILTER_H_ */

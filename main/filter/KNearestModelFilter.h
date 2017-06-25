/*
 * KNearestModelFilter.h
 *
 * 	KNearest neighbor model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef KNNMODELFILTER_H_
#define KNNMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "StatModelFilter.h"

namespace dsa {

class KNearestModelFilter : public StatModelFilter {

	int K_;
	CvKNearest knn_;

	void predict();

public:

	KNearestModelFilter(const char* filename, const char* name=0, int K=10);
	virtual ~KNearestModelFilter();
};

}
#endif /* KNNMODELFILTER_H_ */

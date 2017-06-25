/*
 * SVMModelFilter.h
 *
 * 	Support vector machine model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef SVMMODELFILTER_H_
#define SVMMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "StatModelFilter.h"

namespace dsa {

class SVMModelFilter : public StatModelFilter {

	CvSVM svm_;

	void predict();

public:

	SVMModelFilter(const char* filename, const char* name=0);
	virtual ~SVMModelFilter();
};

}
#endif /* SVMMODELFILTER_H_ */

/*
 * ERTModelFilter.h
 *
 * 	Extremely randomized Random trees model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef ERTMODELFILTER_H_
#define ERTMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "StatModelFilter.h"

namespace dsa {

class ERTModelFilter : public StatModelFilter {

	CvERTrees erts_;

	void predict();

public:

	ERTModelFilter(const char* filename, const char* name=0);
	virtual ~ERTModelFilter();
};

}
#endif /* ERTMODELFILTER_H_ */

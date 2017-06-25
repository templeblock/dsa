/*
 * DTModelFilter.h
 *
 * 	Decision Tree model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef DTMODELFILTER_H_
#define DTMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "StatModelFilter.h"

namespace dsa {

class DTModelFilter : public StatModelFilter {

	CvDTree tree_;

	void predict();

public:

	DTModelFilter(const char* filename, const char* name=0);
	virtual ~DTModelFilter();
};

}
#endif /* DTMODELFILTER_H_ */

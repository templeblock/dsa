/*
 * MLPModelFilter.h
 *
 * 	Multi-Layer Perceptron model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef MLPMODELFILTER_H_
#define MLPMODELFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "StatModelFilter.h"

namespace dsa {

class MLPModelFilter : public StatModelFilter {

	CvANN_MLP mlp_;

	void predict();

public:

	MLPModelFilter(const char* filename, const char* name=0);
	virtual ~MLPModelFilter();
};

}
#endif /* MLPMODELFILTER_H_ */

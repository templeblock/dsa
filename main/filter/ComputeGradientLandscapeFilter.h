/*
 * ComputeGradientLandscapeFilter.cpp
 *
 * Compute the gradient landscape of an image
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef COMPUTEGRADIENTLANDSCAPEFILTER_H_
#define COMPUTEGRADIENTLANDSCAPEFILTER_H_

#include <string>
#include <iostream>
#include <fstream>
#include "EvaluationFilter.h"

namespace dsa {

class ComputeGradientLandscapeFilter : public EvaluationFilter {

	size_t ksize_;
	cv::Mat minGradient, maxGradient, meanGradient, medianGradient;
	std::ofstream outfile_;

	void init();
	void evaluate();

public:

	ComputeGradientLandscapeFilter(size_t ksize, std::string &filename);
	virtual ~ComputeGradientLandscapeFilter();
};

}
#endif /* COMPUTEGRADIENTLANDSCAPEFILTER_H_ */

/*
 * HOGFilter.h
 *
 * 	Compute HOG for images
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef HOGFILTER_H_
#define HOGFILTER_H_

#include <opencv2/opencv.hpp>
#include "EvaluationFilter.h"

namespace dsa {

class HOGFilter : public EvaluationFilter {

	int dSize_;
	cv::Size winSize_;
	cv::HOGDescriptor hog_;

	void init();
	void evaluate();

public:

	std::vector<cv::Mat> *segments;
	cv::Mat descriptorValues;

	HOGFilter(cv::Size winSize=cv::Size(64, 128));
	virtual ~HOGFilter();
};

}
#endif /* HOGFILTER_H_ */

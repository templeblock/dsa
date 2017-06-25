/*
 * LocalizationEvaluationFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/histograms.html#equalizehist
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef LOCALIZATIONEVALUATIONFILTER_H_
#define LOCALIZATIONEVALUATIONFILTER_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/thread.hpp>
#include <aruco/aruco.h>
#include <generic/definitions.h>
#include "EvaluationFilter.h"
#include "helper/WaitForInputWorker.h"

namespace dsa {

class LocalizationEvaluationFilter : public EvaluationFilter {

	bool toggle_;
	int toggleCount_;
	std::ofstream outfile_;
	boost::thread *listenForInputThread_;
	WaitForInputWorker *worker_;

	void init();
	void evaluate();

public:

	bool* haveValue1;
	bool* haveValue2;
	bool* haveValue3;
	clock_t* timestamp;
	float*	forkHeight;
	float*	forkRotation;
	cv::Point2f* forkEndPosition;
	cv::Mat* camPosition;
	cv::Mat* camRotation;

	LocalizationEvaluationFilter(std::string &filename);
	virtual ~LocalizationEvaluationFilter();
};

}
#endif /* LOCALIZATIONEVALUATIONFILTER_H_ */

/*
 * MeanshiftFilter.h
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef MEANSHIFTFILTER_H_
#define MEANSHIFTFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "EvaluationFilter.h"

namespace dsa {

class MeanshiftFilter : public EvaluationFilter {

	bool initialized_;
	std::vector<cv::Rect> trackWindows_;
	std::vector<cv::Mat> histograms_;

	void init();
	void evaluate();

public:

	MeanshiftFilter(std::vector<cv::Mat> &histograms, std::vector<cv::Rect> &rois);
	virtual ~MeanshiftFilter();
};

}
#endif /* MEANSHIFTFILTER_H_ */

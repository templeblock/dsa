/*
 * CamshiftFilter.h
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef CAMSHIFTFILTER_H_
#define CAMSHIFTFILTER_H_

#include <opencv2/opencv.hpp>
#include <generic/types.h>
#include "EvaluationFilter.h"

namespace dsa {

class CamshiftFilter : public EvaluationFilter {

	std::vector<cv::Mat> histograms_;
	std::vector<cv::Rect> rois_;

	void init();
	void evaluate();

public:

	CamshiftFilter(std::vector<cv::Mat> &histograms, std::vector<cv::Rect> &rois);
	virtual ~CamshiftFilter();
};

}
#endif /* CAMSHIFTFILTER_H_ */

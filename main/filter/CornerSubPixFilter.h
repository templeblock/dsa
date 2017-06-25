/*
 * CornerSubPixFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#cornersubpix
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef CORNERSUBPIXFILTER_H_
#define CORNERSUBPIXFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class CornerSubPixFilter : public EvaluationFilter {

	cv::Mat corners_;
	cv::Size winSize_;
	cv::Size zeroZone_;
	cv::TermCriteria criteria_;

	void init();
	void evaluate();

public:

	cv::Mat* corners;
	cv::Size* winSize;
	cv::Size* zeroZone;
	cv::TermCriteria* criteria;

	CornerSubPixFilter(cv::Mat& corners, cv::Size winSize, cv::Size zeroZone, cv::TermCriteria& criteria);
	virtual ~CornerSubPixFilter();

};

}
#endif /* CORNERSUBPIXFILTER_H_ */

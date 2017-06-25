/*
 * FindContoursFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html#findcontours
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef FINDCONTOURSFILTER_H_
#define FINDCONTOURSFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class FindContoursFilter : public EvaluationFilter {

	std::vector<std::vector<cv::Point> > contours_;
	int mode_;
	int method_;
	cv::Point offset_;

	void init();
	void evaluate();

public:

	std::vector<std::vector<cv::Point> >* contours;
	int* mode;
	int* method;
	cv::Point* offset;

	FindContoursFilter(std::vector<std::vector<cv::Point> > &contours, int mode, int method, cv::Point offset=cv::Point());
	virtual ~FindContoursFilter();

};

}
#endif /* FINDCONTOURSFILTER_H_ */

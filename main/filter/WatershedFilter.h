/*
 * WatershedFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#watershed
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef WATERSHEDFILTER_H_
#define WATERSHEDFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class WatershedFilter : public EvaluationFilter {

	cv::Mat markers_;

	void init();
	void evaluate();

public:

	cv::Mat* markers;

	WatershedFilter(cv::Mat& markers);
	virtual ~WatershedFilter();

};

}
#endif /* WATERSHEDFILTER_H_ */

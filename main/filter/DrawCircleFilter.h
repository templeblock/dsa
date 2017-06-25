/*
 * DrawCircleFilter.h
 *
 *	Draw a number of circles to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef DRAWCIRCLEFILTER_H_
#define DRAWCIRCLEFILTER_H_

#include "DrawFilter.h"
#include <generic/types.h>

namespace dsa {

class DrawCircleFilter : public DrawFilter {

	std::vector<Circle> circles_;

	virtual void init();
	virtual void draw();

public:

	std::vector<Circle>* circles;

	DrawCircleFilter(std::vector<Circle>& circles, cv::Scalar color=cv::Scalar(127), UINT32 everyNth=1);
	virtual ~DrawCircleFilter();
};

}
#endif /* DRAWCIRCLEFILTER_H_ */

/*
 * DrawLineFilter.h
 *
 *	Draw a number of lines to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef DRAWLINEFILTER_H_
#define DRAWLINEFILTER_H_

#include "DrawFilter.h"

namespace dsa {

class DrawLineFilter : public DrawFilter {

	std::vector<cv::Vec4i> lines_;

	virtual void init();
	virtual void draw();

public:

	std::vector<cv::Vec4i>* lines;

	DrawLineFilter(std::vector<cv::Vec4i>& lines, cv::Scalar color=cv::Scalar(127), UINT32 everyNth=1);
	virtual ~DrawLineFilter();
};

}
#endif /* DRAWLINEFILTER_H_ */

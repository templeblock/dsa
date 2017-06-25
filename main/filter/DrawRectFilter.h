/*
 * DrawRectFilter.h
 *
 *	Draw a number of rectangles to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef DRAWRECTFILTER_H_
#define DRAWRECTFILTER_H_

#include "DrawFilter.h"

namespace dsa {

class DrawRectFilter : public DrawFilter {

	std::vector<cv::Rect> rectangles_;

	virtual void init();
	virtual void draw();

public:

	std::vector<cv::Rect>* rectangles;

	DrawRectFilter(std::vector<cv::Rect>& rectangles, cv::Scalar color=cv::Scalar(127), UINT32 everyNth=1);
	virtual ~DrawRectFilter();
};

}
#endif /* DRAWRECTFILTER_H_ */

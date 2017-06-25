/*
 * DrawContourFilter.h
 *
 *	Draw contours to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef DRAWCONTOURFILTER_H_
#define DRAWCONTOURFILTER_H_

#include "DrawFilter.h"

namespace dsa {

class DrawContourFilter : public DrawFilter {

	std::vector<std::vector<cv::Point> > contours_;

	virtual void init();
	virtual void draw();

public:

	std::vector<std::vector<cv::Point> >* contours;

	DrawContourFilter(std::vector<std::vector<cv::Point> >& contours, cv::Scalar color=cv::Scalar(127), UINT32 everyNth=1);
	virtual ~DrawContourFilter();
};

}
#endif /* DRAWCONTOURFILTER_H_ */

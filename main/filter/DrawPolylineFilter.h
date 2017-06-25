/*
 * DrawPolylineFilter.h
 *
 *	Draw a number of polylines to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef DRAWPOLYLINEFILTER_H_
#define DRAWPOLYLINEFILTER_H_

#include "DrawFilter.h"

namespace dsa {

class DrawPolylineFilter : public DrawFilter {

	std::vector<cv::Mat> polylines_;

	virtual void init();
	virtual void draw();

public:

	std::vector<cv::Mat>* polylines;

	DrawPolylineFilter(std::vector<cv::Mat> polylines, cv::Scalar color=cv::Scalar(127), UINT32 everyNth=1);
	virtual ~DrawPolylineFilter();

	void setPolylines(std::vector<cv::Mat> polylines);
};

}
#endif /* DRAWPOLYLINEFILTER_H_ */

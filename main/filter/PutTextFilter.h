/*
 * PutTextFilter.h
 *
 * 	See
 *
 *	http://opencv.willowgarage.com/documentation/cpp/drawing_functions.html#cv-puttext
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef PUTTEXTFILTER_H_
#define PUTTEXTFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class PutTextFilter : public ImageFilter {

	std::string text_;
	cv::Point org_;
	int fontFace_;
	double fontScale_;
	cv::Scalar color_;
	int thickness_;
	int lineType_;
	bool bottomLeftOrigin_;

	void init();

public:

	std::string *text;
	cv::Point *org;
	cv::Scalar *color;

	PutTextFilter(const std::string text, cv::Point org, int fontFace, double fontScale, cv::Scalar color, int thickness=1, int lineType=8, bool bottomLeftOrigin=false);
	virtual ~PutTextFilter();

	void apply();
};

}
#endif /* PUTTEXTFILTER_H_ */

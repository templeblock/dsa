/*
 * GetRectSubPixFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#getrectsubpix
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef GETRECTSUBPIXFILTER_H_
#define GETRECTSUBPIXFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class GetRectSubPixFilter : public ImageFilter {

	cv::Size patchSize_;
	cv::Point2f center_;
	int patchType_;

	void init();

public:

	cv::Size* patchSize;
	cv::Point2f* center;
	int* patchType;
	GetRectSubPixFilter(cv::Size patchSize, cv::Point2f center, int patchType=-1);
	virtual ~GetRectSubPixFilter();

	void apply();
};

}
#endif /* GETRECTSUBPIXFILTER_H_ */

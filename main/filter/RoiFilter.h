/*
 * RoiFilter.h
 *
 * 	Extract a region of interest defined by a rectangle.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef ROIFILTER_H_
#define ROIFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class RoiFilter : public ImageFilter {

	cv::Rect roi_;

	void init();

public:

	cv::Rect* roi;

	RoiFilter(cv::Rect rect);
	virtual ~RoiFilter();

	void apply();
	void setRoi(cv::Rect rect);
	bool isReady() const;
};

}
#endif /* ROIFILTER_H_ */

/*
 * DrawFilter.h
 *
 *	BaseClass to draw shapes to an existing image without making any structural changes
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef DRAWFILTER_H_
#define DRAWFILTER_H_

#include <generic/types.h>
#include "ImageFilter.h"

namespace dsa {

class DrawFilter : public ImageFilter {

	bool trigger_;
	UINT32 everyNth_;
	int run_;
	cv::Scalar color_;

	virtual void init();
	virtual void draw() = 0;

public:

	bool* trigger;
	UINT32* everyNth;
	cv::Scalar* color;

	DrawFilter(cv::Scalar color, UINT32 everyNth=1);
	virtual ~DrawFilter();

	void apply();
};

}
#endif /* DRAWFILTER_H_ */

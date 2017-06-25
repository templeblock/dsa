/*
 * BackProjectionFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/doc/tutorials/imgproc/histograms/back_projection/back_projection.html
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef BACKPROJECTIONFILTER_H_
#define BACKPROJECTIONFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class BackProjectionFilter : public ImageFilter {

	void init();

public:

	BackProjectionFilter();
	virtual ~BackProjectionFilter();

	void apply();
};

}
#endif /* BACKPROJECTIONFILTER_H_ */

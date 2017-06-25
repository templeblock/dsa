/*
 * ImageFilter.h
 *
 *	Common image filter class. The actual image filters are derived from this class and have to
 *	implement the apply() method which does the magic of the filter on the image.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef IMAGEFILTER_H_
#define IMAGEFILTER_H_

#include "Filter.h"
#include <sensordata/ImageSensorData.h>

namespace dsa {

class ImageFilter: public Filter {

protected:

	virtual void init() = 0;

public:

	cv::Mat* src;	// pointer to the source image
	cv::Mat dst;	// the output image

	ImageFilter();
	virtual ~ImageFilter();

	virtual void preprocessing();
	virtual void apply() = 0;
	virtual void bypass();
	virtual void postprocessing();

	bool isReady() const;
};

}
#endif /* IMAGEFILTER_H_ */

/*
 * GrabImageFilter.cpp
 *
 *	Grabs an image from a Videohub
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef GRABIMAGEFILTER_H_
#define GRABIMAGEFILTER_H_

#include "GrabFilter.h"
#include <sensordata/ImageSensorData.h>
#include <sensordata/VideoHub.h>

namespace dsa {

class GrabImageFilter : public GrabFilter {

protected:

	virtual void init();

public:

	cv::Mat dst;	// the output image

	GrabImageFilter(VideoHub &videoHub);
	virtual ~GrabImageFilter();

	virtual void apply();
	virtual void bypass();
	bool isReady() const;
};

}
#endif /* GRABIMAGEFILTER_H_ */

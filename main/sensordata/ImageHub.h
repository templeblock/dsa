/*
 * VideoHub.h
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#ifndef IMAGEHUB_H_
#define IMAGEHUB_H_

#include "DataHub.h"
#include "ImageSensorData.h"

namespace dsa {

class ImageHub : public DataHub {
protected:
	ImageSensorData data_;

public:
	ImageHub(const cv::Mat& mat);
	virtual ~ImageHub();

	virtual AbstractSensorData* getData(const bool copy=true);
	virtual void setData(const cv::Mat& mat);
};

}
#endif /* IMAGEHUB_H_ */

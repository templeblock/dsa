/*
 * ImageSensorData.h
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#ifndef IMAGESENSORDATA_H_
#define IMAGESENSORDATA_H_

#include <opencv2/opencv.hpp>
#include "AbstractSensorData.h"

namespace dsa {

class ImageSensorData : public AbstractSensorData {

private:

	cv::Mat image_;	// contains the image we last grabbed

public:
	ImageSensorData(const cv::Mat& mat=cv::Mat());
	ImageSensorData(const cv::Mat& mat, const clock_t grabTimestamp);
	ImageSensorData(const cv::Mat& mat, const clock_t grabTimestamp, const clock_t dataTimestamp);
	ImageSensorData(const ImageSensorData& copy);
	ImageSensorData(const ImageSensorData& copy, const int channel);
	~ImageSensorData();

	cv::Mat& getImage();
};

}
#endif /* IMAGESENSORDATA_H_ */

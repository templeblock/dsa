/**
 * ImageHub.cpp
 *
 *	Derived from the "interface" DataHub. Can be used to serve images within any filter pipeline and hand out as many copies as
 *	required. This is definitely not thread-safe. Use this Hub if you want to serve any type of image or sequence of images other
 *	than coming from a videocapture directly.
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#include "ImageHub.h"

/**
 * Constructor. Initialize contents (image) of the hub.
 *
 * @param mat: image to serve
 *
 */
dsa::ImageHub::ImageHub(const cv::Mat& mat) : DataHub() {
	setData(mat);
}

/**
 * Destructor.
 */
dsa::ImageHub::~ImageHub() {
}


/**
 * Returns a copy of the image.
 *
 * @param copy return a copy of the data (true) or only a reference to the data (false)
 * @return the captured ImageSensorData
 */
dsa::AbstractSensorData* dsa::ImageHub::getData(const bool copy) {
	ImageSensorData* const data = (copy) ? new ImageSensorData(ImageSensorData(data_)) : new ImageSensorData(data_);

	return(data);
}

/**
 * Set/change contents (image) to serve.
 *
 * @param mat: the image
 * @return the ImageSensorData requested (either old or new)
 */
void dsa::ImageHub::setData(const cv::Mat& mat) {
	grabData();
	data_ = ImageSensorData(mat);
}

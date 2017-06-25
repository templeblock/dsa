/**
 * Derived from AbstractSensorData class. Adds ability to return copies its (class-member) image. An instance of this
 * class will contain one image from a specific point in time.
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#include "ImageSensorData.h"

/**
 * Constructor. Creates an instance of this class initialized with the specified data from the Matrix mat
 *
 * @param mat: the matrix representing an image
 */
dsa::ImageSensorData::ImageSensorData(const cv::Mat& mat) : AbstractSensorData() {
	image_ = mat;
}

/**
 * Constructor. Creates an instance of this class initialized with the specified data from the Matrix mat and dates it to grabTimestamp
 *
 * @param mat
 * @param grabTimestamp
 */
dsa::ImageSensorData::ImageSensorData(const cv::Mat& mat, const clock_t grabTimestamp) : AbstractSensorData(grabTimestamp) {
	image_ = mat;
}

/**
 * Constructor. Creates an instance of this class initialized with the specified data from the Matrix mat and dates it to grabTimestamp and dataTimestamp
 *
 * @param mat
 * @param grabTimestamp
 * @param dataTimestamp
 */
dsa::ImageSensorData::ImageSensorData(const cv::Mat& mat, const clock_t grabTimestamp, const clock_t dataTimestamp) : AbstractSensorData(grabTimestamp, dataTimestamp) {
	image_ = mat;
}

/**
 * Copy constructor. Create a deep-copy of an object of this class (by cloning the image it contains).
 *
 * @param copy
 */
dsa::ImageSensorData::ImageSensorData(const ImageSensorData& copy) : AbstractSensorData(copy) {
	image_ = copy.image_.clone();
}

/**
 * Copy constructor. Create a deep-copy of an object of this class (by cloning the image it contains).
 *
 * @param copy
 * @param channel to copy
 */
dsa::ImageSensorData::ImageSensorData(const ImageSensorData& copy, const int channel) : AbstractSensorData(copy) {
	cv::Mat image(copy.image_.rows, copy.image_.cols, CV_MAKETYPE(copy.image_.depth(), 1));
	int from_to[] = { channel, 0 };
	mixChannels(&copy.image_, 1, &image, 1, from_to, 1);	// ob das richtig ist?
	image_ = image;
}

/**
 * Destructor.
 */
dsa::ImageSensorData::~ImageSensorData() {
}

/**
 * Return the image contained in this object.
 *
 * @return the Matrix representing the image.
 */
cv::Mat& dsa::ImageSensorData::getImage() {
	return(image_);
}

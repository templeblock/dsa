/*
 * DirectShowConfigurer.cpp
 *
 * See the following URLs for the set of settings supported by opencv
 *
 * http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-get
 * http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-set
 *
 *  Created on: 25.09.2012
 *      Author: jung
 */

#include "DirectShowConfigurer.h"

/**
 * Constructor
 *
 * @param capture:
 */
dsa::DirectShowConfigurer::DirectShowConfigurer(cv::VideoCapture* capture) {
	// it would be desireable for the names to comply with the genicam SFNC, so change this where possible, later.
	map_["Width"] = CV_CAP_PROP_FRAME_WIDTH;		// genicam-compliant
	map_["Height"] = CV_CAP_PROP_FRAME_HEIGHT;		// genicam-compliant
	map_["AcquisitionFrameRate"] = CV_CAP_PROP_FPS;	// genicam-compliant
	map_["Fourcc"] = CV_CAP_PROP_FOURCC;
	map_["Format"] = CV_CAP_PROP_FORMAT;
	map_["AcquisitionMode"] = CV_CAP_PROP_MODE;		// genicam-compliant
	map_["Brightness"] = CV_CAP_PROP_BRIGHTNESS;	// probably no match available
	map_["Contrast"] = CV_CAP_PROP_CONTRAST;		// probably no match available
	map_["Saturation"] = CV_CAP_PROP_SATURATION;	// probably no match available
	map_["Hue"] = CV_CAP_PROP_HUE;					// probably no match available
	map_["Gain"] = CV_CAP_PROP_GAIN;				// genicam-compliant
	map_["ExposureTime"] = CV_CAP_PROP_EXPOSURE;	// genicam-compliant
	map_["Rectification"] = CV_CAP_PROP_RECTIFICATION;

	capture_ = capture;
}

/**
 * Destructor
 */
dsa::DirectShowConfigurer::~DirectShowConfigurer() {
}

/**
 * Get camera property by its ID. See the header of this file for a link to the properties available.
 *
 * @param propertyId: ID of the property
 *
 * @return value of the property
 */
double dsa::DirectShowConfigurer::get(const int propertyId) {
	return(capture_->get(propertyId));
}

/**
 * Set camera property by its Id to a given value. See the header of this file for a link to the properties available.
 *
 * @param propertyId: Id of the property
 * @param value: the value to set the property to
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::set(const int propertyId, const double value) {
	return(capture_->set(propertyId, value));
}

/**
 * Get camera property by its name.
 *
 * @param property: name of the property
 *
 * @return value of the property
 */
double dsa::DirectShowConfigurer::get(std::string& property) {
	return(capture_->get(map_[property]));
}

/**
 * Set camera property by its name to a given value.
 *
 * @param property: name of the property
 * @param value: the value to set the property to
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::set(std::string& property, const double value) {
	return(capture_->set(map_[property], value));
}

/**
 * Get frame height
 *
 * @return height in pixels
 */
int dsa::DirectShowConfigurer::getHeight() {
	return((int)(capture_->get(CV_CAP_PROP_FRAME_HEIGHT)));
}

/**
 * Set frame height in pixels
 *
 * @param value: number of pixels
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setHeight(const int value) {
	return(capture_->set(CV_CAP_PROP_FRAME_HEIGHT, (double)value));
}

/**
 * Get frame width
 *
 * @return width in pixels
 */
int dsa::DirectShowConfigurer::getWidth() {
	return((int)(capture_->get(CV_CAP_PROP_FRAME_WIDTH)));
}

/**
 * Set frame width in pixels
 *
 * @param value: number of pixels
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setWidth(const int value) {
	return(capture_->set(CV_CAP_PROP_FRAME_WIDTH, (double)value));
}

/**
 * Get number of frames per second
 *
 * @return fps
 */
double dsa::DirectShowConfigurer::getFps() {
	return((int)(capture_->get(CV_CAP_PROP_FPS)));
}

/**
 * Set number of frames per second
 *
 * @param value: fps
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setFps(const double value) {
	return(capture_->set(CV_CAP_PROP_FPS, value));
}

/**
 * Get format of the Mat objects returned by retrieve(), see OpenCV documentation for details.
 *
 * @return the format
 */
int dsa::DirectShowConfigurer::getFormat() {
	return((int)(capture_->get(CV_CAP_PROP_FORMAT)));
}

/**
 * Set format of the Mat objects returned by retrieve(), see OpenCV documentation for details.
 *
 * @param value: the format
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setFormat(const int value) {
	return(capture_->set(CV_CAP_PROP_FORMAT, (double)value));
}

/**
 * Backend-specific value indicating the current capture mode. No idea what this means for DirectShow, maybe DirectShow docs can clarify this.
 *
 * @return the mode
 */
int dsa::DirectShowConfigurer::getMode() {
	return((int)(capture_->get(CV_CAP_PROP_MODE)));
}

/**
 * Set the current capture mode. No idea what this means for DirectShow, maybe DirectShow docs can clarify this.
 *
 * @param value: the format
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setMode(const int value) {
	return(capture_->set(CV_CAP_PROP_MODE, value));
}

/**
 * Id of the 4-character code of the video codec (determined by CV_FOURCC()).
 *
 * @return the character code
 */
int dsa::DirectShowConfigurer::getFourcc() {
	return((int)(capture_->get(CV_CAP_PROP_FOURCC)));
}

/**
 * Set video codec used.
 *
 * @param value: Id of the 4-character code of the video codec (determined by CV_FOURCC()).
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setFourcc(const int value) {
	return(capture_->set(CV_CAP_PROP_FOURCC, (double)value));
}

/**
 * Get image brightness setting.
 *
 * @return the brightness value
 */
double dsa::DirectShowConfigurer::getBrightness() {
	return((int)(capture_->get(CV_CAP_PROP_BRIGHTNESS)));
}

/**
 * Set image brightness
 *
 * @param value: the brightness value
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setBrightness(const double value) {
	return(capture_->set(CV_CAP_PROP_BRIGHTNESS, value));
}

/**
 * Get image contrast setting.
 *
 * @return the contrast value
 */
double dsa::DirectShowConfigurer::getContrast() {
	return((int)(capture_->get(CV_CAP_PROP_CONTRAST)));
}

/**
 * Set the image contrast.
 *
 * @param value: the contrast value
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setContrast(const double value) {
	return(capture_->set(CV_CAP_PROP_CONTRAST, value));
}

/**
 * Get image saturation setting.
 *
 * @return the saturation value
 */
double dsa::DirectShowConfigurer::getSaturation() {
	return((int)(capture_->get(CV_CAP_PROP_SATURATION)));
}

/**
 * Set image saturation value.
 *
 * @param value: the saturation value
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setSaturation(const double value) {
	return(capture_->set(CV_CAP_PROP_SATURATION, value));
}

/**
 * Get image hue setting.
 *
 * @return the hue value
 */
double dsa::DirectShowConfigurer::getHue() {
	return((int)(capture_->get(CV_CAP_PROP_HUE)));
}

/**
 * Set image hue value.
 *
 * @param value: the hue value
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setHue(const double value) {
	return(capture_->set(CV_CAP_PROP_HUE, value));
}

/**
 * Get camera gain setting.
 *
 * @return the gain value
 */

double dsa::DirectShowConfigurer::getGain() {
	return((int)(capture_->get(CV_CAP_PROP_GAIN)));
}

/**
 * Set camera gain value.
 *
 * @param value: the gain value
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setGain(const double value) {
	return(capture_->set(CV_CAP_PROP_GAIN, value));
}

/**
 * Get camera exposure setting.
 *
 * @return the exposure value
 */
double dsa::DirectShowConfigurer::getExposure() {
	return((int)(capture_->get(CV_CAP_PROP_EXPOSURE)));
}

/**
 * Set camera exposure value
 *
 * @param value: the exposure value
 *
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setExposure(const double value) {
	return(capture_->set(CV_CAP_PROP_EXPOSURE, value));
}

/**
 * Get camera rectification setting (for stereo cameras).
 *
 * @return the brightness value
 */
double dsa::DirectShowConfigurer::getRectification() {
	return((int)(capture_->get(CV_CAP_PROP_RECTIFICATION)));
}

/**
 * Set camera rectification setting (for stereo cameras).
 *
 * @param value: unknown
 * @return true if successful, false otherwise
 */
bool dsa::DirectShowConfigurer::setRectification(const double value) {
	return(capture_->set(CV_CAP_PROP_RECTIFICATION, value));
}

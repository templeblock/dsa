/*
 * DirectShowConfigurer.h
 *
 *  Created on: 25.09.2012
 *      Author: jung
 */

#ifndef DIRECTSHOWCONFIGURER_H_
#define DIRECTSHOWCONFIGURER_H_

#include <map>
#include <opencv2/opencv.hpp>
#include "CameraConfigurer.h"

namespace dsa {

class DirectShowConfigurer : public CameraConfigurer {

	std::map<std::string, int> map_;
	cv::VideoCapture* capture_;

public:

	DirectShowConfigurer(cv::VideoCapture* capture);
	virtual ~DirectShowConfigurer();

	// get/set any property by Id
	double get(const int propertyId);
	bool set(const int propertyId, const double value);
	// get/set property by it's name
	double get(std::string& property);
	bool set(std::string& property, const double value);

	// commonly used properties directly accessible
	// height
	int getHeight();
	bool setHeight(const int value);
	// width
	int getWidth();
	bool setWidth(const int value);
	// frames per second
	double getFps();
	bool setFps(const double value);
	// video codec (see CV_FOURCC() at http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videowriter-videowriter)
	int getFourcc();
	bool setFourcc(const int value);
	// format of the Mat() returned by retrieve
	int getFormat();
	bool setFormat(const int value);
	// mode (backend-specific value indicating the current capture mode)
	int getMode();
	bool setMode(const int value);
	// brightness
	double getBrightness();
	bool setBrightness(const double value);
	// contrast
	double getContrast();
	bool setContrast(const double value);
	// saturation
	double getSaturation();
	bool setSaturation(const double value);
	// hue
	double getHue();
	bool setHue(const double value);
	// gain
	double getGain();
	bool setGain(const double value);
	// exposure
	double getExposure();
	bool setExposure(const double value);
	// rectification (stereo cameras)
	double getRectification();
	bool setRectification(const double value);

};

}
#endif /* DIRECTSHOWCONFIGURER_H_ */

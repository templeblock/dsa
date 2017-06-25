/*
 * CameraConfigurer.h
 *
 *  Created on: 25.09.2012
 *      Author: jung
 */

#ifndef CAMERACONFIGURER_H_
#define CAMERACONFIGURER_H_

#include <string>

namespace dsa {

class CameraConfigurer {

protected:

	enum Protocol { GENICAM, DIRECTSHOW };
	int protocolVersion_;

public:

	CameraConfigurer();
	virtual ~CameraConfigurer();

	// we need a function to return the supported values
	// insert here later!

	// get/set any property by Id
	virtual double get(const int propertyId) = 0;
	virtual bool set(const int propertyId, const double value) = 0;
	// get/set property by it's name
	virtual double get(const std::string property) = 0;
	virtual bool set(const std::string property, const double value) = 0;

	// commonly used properties directly accessible
	// height
	virtual int getHeight() = 0;
	virtual bool setHeight(const int value) = 0;
	// width
	virtual int getWidth() = 0;
	virtual bool setWidth(const int value) = 0;
	// frames per second
	virtual double getFps() = 0;
	virtual bool setFps(const double value) = 0;
	// video codec (see CV_FOURCC() at http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videowriter-videowriter)
	virtual int getFourcc() = 0;
	virtual bool setFourcc(const int value) = 0;
	// brightness
	virtual double getBrightness() = 0;
	virtual bool setBrightness(const double value) = 0;
	// contrast
	virtual double getContrast() = 0;
	virtual bool setContrast(const double value) = 0;
	// saturation
	virtual double getSaturation() = 0;
	virtual bool setSaturation(const double value) = 0;
	// hue
	virtual double getHue() = 0;
	virtual bool setHue(const double value) = 0;
	// gain
	virtual double getGain() = 0;
	virtual bool setGain(const double value) = 0;
	// exposure
	virtual double getExposure() = 0;
	virtual bool setExposure(const double value) = 0;
	// white balance
	virtual double setWhiteBalance() = 0;
	virtual bool setWhiteBalance(const double value) = 0;
	// rectification (stereo cameras)
	virtual double setRectification() = 0;
	virtual bool setRectification(const double value) = 0;

};

}
#endif /* CAMERACONFIGURER_H_ */

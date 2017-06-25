/*
 * VideoHub.h
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#ifndef VIDEOHUB_H_
#define VIDEOHUB_H_

#include "DataHub.h"
#include "ImageSensorData.h"
#include "StrobeController.h"
#include "../../libs/raspi_gpgpu/raspi_config.h"

#ifdef WITH_RASPICAM
	#include <raspicam_cv.h>
#endif

namespace dsa {

class VideoHub : public DataHub {
protected:
	bool activateLamp_;
	bool haveStrobe_;
	#ifdef WITH_RASPICAM
	raspicam::RaspiCam_Cv* capture_;
	#else
	cv::VideoCapture* capture_;
	#endif
	StrobeController strobe_;
	ImageSensorData original_;
	cv::Mat dst_;
   int _lastIdx;

	void grabData();

public:

	bool *activateLamp;
	#ifdef WITH_RASPICAM
	VideoHub(raspicam::RaspiCam_Cv* const capture);
	#else
	VideoHub(cv::VideoCapture* const capture);
	#endif
   VideoHub(){};

	virtual ~VideoHub();

	virtual AbstractSensorData* getData(const bool copy=true);
	virtual AbstractSensorData* getData(const int channel, const bool copy=true);
	virtual AbstractSensorData* getData(const clock_t clock_t, const bool copy=true);
	virtual AbstractSensorData* getData(const clock_t clock_t, const int channel, const bool copy=true);

	#ifdef WITH_RASPICAM
	virtual raspicam::RaspiCam_Cv* getCapture() const;
	#else
	virtual cv::VideoCapture* getCapture() const;
	#endif
	virtual void setStrobeController(StrobeController &strobeController);
};

}
#endif /* VIDEOHUB_H_ */

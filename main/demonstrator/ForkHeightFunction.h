/*
 * ForkHeightFunction.h
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#ifndef FORKHEIGHTFUNCTION_H_
#define FORKHEIGHTFUNCTION_H_

#include "Sensorfunction.h"
#include "ForkHeightResult.h"
#include <demonstrator/localizor/Settings.hpp>
#include <opencv2/opencv.hpp>

#include <sensordata/VideoHub.h>
#include <sensordata/autoadjust/MeanAutoExposureAlgorithm.h>
#include <sensordata/autoadjust/HistAutoExposureAlgorithm.h>
#include <filter/FilterChain.h>
#include <filter/GrabImageFilter.h>
#include <filter/RotationFilter.h>
#include <filter/EmptyMaskFilter.h>
#include <filter/UndistortFilter.h>
#include <filter/AutoWhiteBalanceFilter.h>
#include <filter/FindMarkersFilter.h>
#include <filter/SplitMarkersByIdFilter.h>
#include <filter/EstimatePoseFilter.h>
#include <filter/EmjaysFancyForkPoseFilter.h>
#include <filter/MarkerAOIFilter.h>
#include <filter/DrawCircleFilter.h>
#include <filter/RoiFilter.h>
#include <filter/AutoSensorFilter.h>
#include <filter/ResizeFilter.h>
#include <filter/VideoWriterFilter.h>
#include <filter/ImageOutputFilter.h>
#include <filter/WaitFilter.h>

#ifdef WITH_RASPICAM
	#include <raspicam_cv.h>
#endif

namespace dsa {

class ForkHeightFunction: public dsa::Sensorfunction {
	bool stopped_;

	int exposure_;	// exposure time in microseconds, negative value => autoexposure
	int width_;		// resize to width (pixels)
	int height_;	// resize to height (pixels)
	double fps_;	// slow down to fps computation is too fast
	double rotation_;	// rotate image to angle (degrees)
	double gain_;	// set gain to value, negative values => leave as is

	#ifdef WITH_RASPICAM
	raspicam::RaspiCam_Cv* capture_;
	#else
	cv::VideoCapture *capture_;	// video capture object
	#endif
	ForkHeightResult result_;	// result determined by the sensorfunction
	ForkHeightResult lastValid_;	// last valid result

public:
	#ifdef WITH_RASPICAM
	ForkHeightFunction(raspicam::RaspiCam_Cv* capture, double fps=15.0, int exposure=-1, int width=-1, int height=-1, double rotation=0, double gain=-1.0);
	#else
	ForkHeightFunction(cv::VideoCapture *capture, double fps=15.0, int exposure=-1, int width=-1, int height=-1, double rotation=0, double gain=-1.0);
	#endif
	virtual ~ForkHeightFunction();

	void run();
	void stop();
	bool calibrate();
	SensorfunctionResult* getLastValid();
	SensorfunctionResult* getResult();
};

} /* namespace dsa */

#endif /* FORKHEIGHTFUNCTION_H_ */

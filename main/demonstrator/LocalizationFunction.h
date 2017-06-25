/*
 * LocalizationFunction.h
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#ifndef LOCALIZATIONFUNCTION_H_
#define LOCALIZATIONFUNCTION_H_

#include "Sensorfunction.h"
#include "LocalizationResult.h"
#include <demonstrator/localizor/Settings.hpp>
#include <opencv2/opencv.hpp>
#include "../../libs/raspi_gpgpu/raspi_config.h"

#include <sensordata/VideoHub.h>
#include <sensordata/autoadjust/MeanAutoExposureAlgorithm.h>
#include <filter/FilterChain.h>
#include <filter/CoordinateTransformFilter.h>
#include <filter/GrabImageFilter.h>
#include <filter/FindMarkersFilter.h>
#include <filter/SplitMarkersByIdFilter.h>
#include <filter/EstimatePoseFilter.h>
#include <filter/ResizeFilter.h>
#include <filter/UndistortFilter.h>
#include <filter/WaitFilter.h>
#ifdef WITH_FPGA
   	#include <filter/FPGAFilter.h>
  
#endif

#ifdef WITH_RASPICAM
	#include <raspicam_cv.h>
	#include <filter/ImageServerFilter.h>
#else
	#include <filter/ImageOutputFilter.h>
#endif


namespace dsa {

class LocalizationFunction: public dsa::Sensorfunction {
	bool stopped_;
	int exposure_;	// exposure time in microseconds, negative value => autoexposure
	int width_;		// resize to width (pixels)
	int height_;	// resize to height (pixels)
	double fps_;	// slow down to fps computation is too fast
	double rotation_;	// rotate image to angle (degrees)
	double gain_;	// set gain to value, negative values => leave as is
	std::string config_; // string with path for configuration of camera
	Settings settings_; // settings of camera
	aruco::CameraParameters camparams_; // aruco object with camera parameters

	#ifdef WITH_RASPICAM
		raspicam::RaspiCam_Cv* capture_;
	#else
		cv::VideoCapture *capture_;	// video capture object
	#endif
	LocalizationResult result_;	// result determined by the sensorfunction
	LocalizationResult lastValid_;	// last valid result
	VideoHub* phub_;
	FilterChain *filterChain_;			    // StoreChain Collection
	EstimatePoseFilter	*pfilter5_;
	GrabImageFilter		*pfilter0_;
	FindMarkersFilter	*pfilter3_;
	CoordinateTransformFilter *pfilter6_;

	cv::Mat trans_;
	cv::Mat rot_;

public:
	#ifdef WITH_RASPICAM
	LocalizationFunction(raspicam::RaspiCam_Cv* capture, double fps=15.0, int exposure=-1, int width=-1, int height=-1, double rotation=0, double gain=-1.0);
	#else
	LocalizationFunction(cv::VideoCapture *capture, double fps=15.0, int exposure=-1, int width=-1, int height=-1, double rotation=0, double gain=-1.0);
	#endif
	virtual ~LocalizationFunction();

	void init();
	void run();
	void stop();
	void loadConfig();
	bool calibrate();
	void InitialiseFilters();
	SensorfunctionResult* getLastValid();
	SensorfunctionResult* getResult();
};

} /* namespace dsa */

#endif /* LOCALIZATIONFUNCTION_H_ */

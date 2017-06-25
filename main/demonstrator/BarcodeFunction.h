/*
 * BarcodeFunction.h
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#ifndef BARCODEFUNCTION_H_
#define BARCODEFUNCTION_H_

#include "Sensorfunction.h"
#include "BarcodeResult.h"
#include <demonstrator/localizor/Settings.hpp>
#include <opencv2/opencv.hpp>

#include <sensordata/VideoHub.h>
#include <sensordata/autoadjust/HistAutoExposureAlgorithm.h>
#include <sensordata/autoadjust/MeanAutoExposureAlgorithm.h>
#include <filter/FilterChain.h>
#include <filter/GrabImageFilter.h>
#include <filter/UndistortFilter.h>
#include <filter/AutoWhiteBalanceFilter.h>
#include <filter/BarcodeSegmentationFilter.h>
#include <filter/ZxingBarcodeAnalysisFilter.h>
#include <filter/AutoSensorFilter.h>
#include <filter/CvtColorFilter.h>
#include <filter/VisualizeBarcodeFilter.h>
#include <filter/VideoWriterFilter.h>
#include <filter/ResizeFilter.h>
#include <filter/ImageOutputFilter.h>
#include <filter/WaitFilter.h>

namespace dsa {

class BarcodeFunction : public dsa::Sensorfunction {
	bool stopped_;
	bool awb_;
	int exposure_;	// exposure time in microseconds, negative value => autoexposure
	double fps_;	// slow down to fps computation is too fast
	double gain_;	// set gain to value, negative values => leave as is

	cv::VideoCapture *capture_;	// video capture object
	BarcodeResult result_;	// result determined by the sensorfunction

public:
	BarcodeFunction(cv::VideoCapture *capture, double fps=15.0, int exposure=-1, bool awb=false,  double gain=-1.0);
	virtual ~BarcodeFunction();

	void run();
	void stop();
	bool calibrate();
	SensorfunctionResult* getLastValid();
	SensorfunctionResult* getResult();
};

} /* namespace dsa */

#endif /* BARCODEFUNCTION_H_ */

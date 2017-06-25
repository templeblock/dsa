/*
 * AutoSensorAlgorithm.h
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#ifndef AUTOSENSORALGORITHM_H_
#define AUTOSENSORALGORITHM_H_

#include <generic/types.h>

#ifdef WITH_RASPICAM
	#include <raspicam_cv.h>
#endif

namespace dsa {

static const cv::Mat emptyMask = cv::Mat();

class AutoSensorAlgorithm {

protected:
	int property_;
	int everyNthFrame_;
	int run_;
	double minValue_;
	double maxValue_;
	#ifdef WITH_RASPICAM
	raspicam::RaspiCam_Cv* capture_;
	#else
	cv::VideoCapture* capture_;
	#endif
	cv::Mat mask_;

	virtual bool needsUpdate();
	virtual bool update(cv::Mat &image, cv::Mat &mask) = 0;
public:
	#ifdef WITH_RASPICAM
	AutoSensorAlgorithm(raspicam::RaspiCam_Cv* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame);
	#else
	AutoSensorAlgorithm(cv::VideoCapture* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame);
	#endif
	virtual ~AutoSensorAlgorithm();

	virtual bool run(cv::Mat &image, cv::Mat &mask);
	virtual bool run(cv::Mat &image);
	double getCurrentValue();
};

}
#endif /* AUTOSENSORALGORITHM_H_ */

/*
 * AutoExposureAlgorithm.h
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#ifndef AUTOEXPOSUREALGORITHM_H_
#define AUTOEXPOSUREALGORITHM_H_

#include "AutoSensorAlgorithm.h"

#ifdef WITH_RASPICAM
	#include <raspicam_cv.h>
#endif

namespace dsa {

class AutoExposureAlgorithm : public AutoSensorAlgorithm {

protected:
	int everyNthPixel_;
	double exposureTimeThreshold_;

	virtual double calcError(cv::Mat &image, cv::Mat &mask) = 0;
	virtual bool update(cv::Mat &image, cv::Mat &mask);

public:
	#ifdef WITH_RASPICAM
	AutoExposureAlgorithm(raspicam::RaspiCam_Cv* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel);
	#else
	AutoExposureAlgorithm(cv::VideoCapture* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel);
	#endif
	virtual ~AutoExposureAlgorithm();

	double getExposureTimeThreshold();
	void setExposureTimeThreshold(double value);
};

}
#endif /* AUTOEXPOSUREALGORITHM_H_ */

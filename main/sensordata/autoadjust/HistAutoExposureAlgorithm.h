/*
 * HistAutoExposureAlgorithm.h
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#ifndef HISTAUTOEXPOSUREALGORITHM_H_
#define HISTAUTOEXPOSUREALGORITHM_H_

#include "AutoExposureAlgorithm.h"

#ifdef WITH_RASPICAM
	#include <raspicam_cv.h>
#endif

namespace dsa {

class HistAutoExposureAlgorithm : public AutoExposureAlgorithm {

protected:
	virtual double calcError(cv::Mat &image, cv::Mat &mask);

public:
	#ifdef WITH_RASPICAM
	HistAutoExposureAlgorithm(raspicam::RaspiCam_Cv* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel=1);
	#else
	HistAutoExposureAlgorithm(cv::VideoCapture* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel=1);
	#endif
	virtual ~HistAutoExposureAlgorithm();
};

}
#endif /* HISTAUTOEXPOSUREALGORITHM_H_ */

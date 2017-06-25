/*
 * MeanAutoExposureAlgorithm.h
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#ifndef MEANAUTOEXPOSUREALGORITHM_H_
#define MEANAUTOEXPOSUREALGORITHM_H_

#include "AutoExposureAlgorithm.h"

#ifdef WITH_RASPICAM
	#include <raspicam_cv.h>
#endif


namespace dsa {

class MeanAutoExposureAlgorithm : public AutoExposureAlgorithm {

protected:
	virtual double calcError(cv::Mat &image, cv::Mat &mask);

public:
#ifdef WITH_RASPICAM
	MeanAutoExposureAlgorithm(raspicam::RaspiCam_Cv* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel=1);
#else
	MeanAutoExposureAlgorithm(cv::VideoCapture* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel=1);
#endif
	virtual ~MeanAutoExposureAlgorithm();
};

}
#endif /* MEANAUTOEXPOSUREALGORITHM_H_ */

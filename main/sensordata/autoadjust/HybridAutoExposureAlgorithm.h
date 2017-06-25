/*
 * HybridAutoExposureAlgorithm.cpp
 *
 * The HistAutoExposureAlgorithm preserves the information in the image well by choosing an exposure time to prevent whichever of overexposure or underexposure is worse at the moment.
 * Since overexposure is usually worse, it will favor low exposure times to prevent overexposure, which leads to preservation of information at the cost of a bad signal to noise ratio.
 * This approach is a compromise between preserving information and reducing noise. It features both the HistAutoExposureAlgorithm and MeanAutoExposureAlgorithm and tries fits them
 * together by calculating their errors separately and build the weighted sum over both errors. The weight can either be specified statically via the "weight" parameter, or automatically
 * be determined (see below).
 *
 * A better method would probably be, to specify a desireable noise level (or interval), however the result would be very camera specific and difficult to evaluate. Also specifying the
 * proper noise level could be a pain for systems integrators, therefore such a method is considered out of the focus of our research project.
 *
 *  Created on: 07.12.2012
 *      Author: jung
 */

#ifndef HYBRIDAUTOEXPOSUREALGORITHM_H_
#define HYBRIDAUTOEXPOSUREALGORITHM_H_

#include "AutoExposureAlgorithm.h"

namespace dsa {

class HybridAutoExposureAlgorithm : public AutoExposureAlgorithm {

	double weight_;

protected:
	virtual double calcError(cv::Mat &image, cv::Mat &mask);

public:
	HybridAutoExposureAlgorithm(cv::VideoCapture* const videoCapture, const double minVal, const double maxVal, int property, int everyNthFrame, int everyNthPixel=1, const double weight=-1.0);
	virtual ~HybridAutoExposureAlgorithm();
};

}
#endif /* HYBRIDAUTOEXPOSUREALGORITHM_H_ */

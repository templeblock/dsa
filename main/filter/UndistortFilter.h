/*
 * UndistortFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#undistort
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef UNDISTORTFILTER_H_
#define UNDISTORTFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class UndistortFilter : public ImageFilter {

	bool initialized_;
	int interpolation_;
	int borderMode_;
	cv::Scalar borderValue_;
	cv::Mat cameraMatrix_;
	cv::Mat distCoeffs_;
	cv::Mat newCameraMatrix_;
	cv::Mat map1_;
	cv::Mat map2_;

	void init();

public:
	int* interpolation;
	int* borderMode;
	cv::Scalar* borderValue;

	UndistortFilter(cv::Mat& cameraMatrix, cv::Mat& distCoeffs, const cv::Mat& newCameraMatrix=cv::Mat(), int interpolation=cv::INTER_LINEAR, int borderMode=cv::BORDER_CONSTANT, cv::Scalar borderValue=cv::Scalar());
	virtual ~UndistortFilter();

	void apply();
};

}
#endif /* UNDISTORTFILTER_H_ */

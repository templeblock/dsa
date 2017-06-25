/*
 * UndistortFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#undistort
 *
 *	for details on the image filter.
 *	We made a couple of optimizations like only computing the maps once and only apply them later to achieve more speed.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "UndistortFilter.h"

/**
 * Constructor
 *
 * Initialize the filter to based on the camera parameters.
 *
 * @param cameraMatrix: Input camera matrix.
 * @param distCoeffs: Input vector of distortion coefficients.
 * @param newCameraMatrix: Camera matrix of the distorted image. By default, it is the same as cameraMatrix but you may additionally scale and shift the result by using a different matrix.
 */
dsa::UndistortFilter::UndistortFilter(cv::Mat &cameraMatrix, cv::Mat& distCoeffs, const cv::Mat& newCameraMatrix, int interpolation, int borderMode, cv::Scalar borderValue) {
	initialized_ = false;
	interpolation_ = interpolation;
	borderMode_ = borderMode;
	borderValue_ = borderValue;

	cameraMatrix_ = cameraMatrix;
	distCoeffs_ = distCoeffs;
	newCameraMatrix_ = newCameraMatrix;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::UndistortFilter::~UndistortFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::UndistortFilter::init() {
	interpolation = &interpolation_;
	borderMode = &borderMode_;
	borderValue = &borderValue_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::UndistortFilter::apply() {
	if(isReady()) {
		if(!initialized_) {
			map1_ = cv::Mat(src->rows, src->cols, CV_16SC2);
			map2_ = cv::Mat(src->rows, src->cols, CV_16UC1);

			cv::Mat_<double> A, Ar, I = cv::Mat_<double>::eye(3,3);
			cameraMatrix_.convertTo(A, CV_64F);
			if(distCoeffs_.data)
				distCoeffs_ = cv::Mat_<double>(distCoeffs_);
			else {
				distCoeffs_.create(5, 1, CV_64F);
				distCoeffs_ = 0.;
			}

			(newCameraMatrix_.data) ? newCameraMatrix_.convertTo(Ar, CV_64F) : A.copyTo(Ar);	// A = copy of Ar if no newCameraMatrix given

			initUndistortRectifyMap(A, distCoeffs_, I, Ar, cv::Size(src->cols, src->rows), map1_.type(), map1_, map2_);
			initialized_ = true;
		}
		dst = cv::Mat(src->rows, src->cols, src->type());
		remap(*src, dst, map1_, map2_, *interpolation, *borderMode, *borderValue);	// remap
	}
}

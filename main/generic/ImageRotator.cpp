/*
 * ImageRotator.cpp
 *
 *  Created on: 17.09.2013
 *      Author: jung
 */

#include "ImageRotator.h"

dsa::ImageRotator::ImageRotator(const int interpolation, const int borderMode, const cv::Scalar borderValue) {
	interpolation_ = interpolation;
	borderMode_ = borderMode;
	borderValue_ = borderValue;
}

dsa::ImageRotator::~ImageRotator() {
}

void dsa::ImageRotator::rotate(cv::Mat &src, cv::Mat &dst, double angle, const double scale) {
	cv::Point2f center(src.cols/2.0f, src.rows/2.0f);	// always rotate image around center (thats what this filter is for)

	// calculate scale factor from given target size
	double scaleX = scale; //dsize.width/(double)src.cols;
	double scaleY = scale; //dsize.height/(double)src.rows;

	// map interval [0; 360] -> [-180; 180]
	if(angle > 180.0)
		angle -= 360.0;
	else if(angle < -180.0)
		angle += 360.0;

	// sine and cosine take radians as argument, hence transform degree to radians (*PI/180)
	double radAngle = angle * CV_PI/180.0;
	double sinAngle = sin(radAngle);
	double cosAngle = cos(radAngle);

	// compute rotation matrix. we do not use the getRotationMatrix2D method, as it seems to suffer from a bug when also scaling at the same time (image is not completely within target size)
	cv::Mat m = cv::Mat(2, 3, CV_64F);
	// transformation part
	m.at<double>(0, 0) = scaleX*cosAngle;
	m.at<double>(0, 1) = scaleY*sinAngle;
	m.at<double>(1, 0) = -scaleX*sinAngle;
	m.at<double>(1, 1) = scaleY*cosAngle;
	// offset and target window size
	double xoffset;
	double yoffset;
	cv::Size targetSize;
	if((angle < 45.0 && angle > -45.0) || angle > 135.0 || angle < -135.0) {
		targetSize = cv::Size(scaleX*src.cols, scaleY*src.rows);
		xoffset = scaleX * center.x;
		yoffset = scaleY * center.y;
	} else {
		targetSize = cv::Size(scaleY*src.rows, scaleX*src.cols);
		xoffset = scaleY * center.y;
		yoffset = scaleX * center.x;
	}
	m.at<double>(0, 2) = -scaleX*cosAngle*center.x - scaleY*sinAngle*center.y + xoffset;
	m.at<double>(1, 2) = scaleX*sinAngle*center.x - scaleY*cosAngle*center.y + yoffset;

	// apply matrix to affine transform
	cv::warpAffine(src, dst, m, targetSize, interpolation_, borderMode_, borderValue_);
}

/*
 * ImageRotator.h
 *
 *  Created on: 17.09.2013
 *      Author: jung
 */

#ifndef IMAGEROTATOR_H_
#define IMAGEROTATOR_H_

#include <opencv2/opencv.hpp>

namespace dsa {

class ImageRotator {

	int interpolation_;
	int borderMode_;
	cv::Scalar borderValue_;

public:
	ImageRotator(const int interpolation=cv::INTER_LINEAR, const int borderMode=cv::BORDER_CONSTANT, const cv::Scalar borderValue=cv::Scalar());
	virtual ~ImageRotator();

	void rotate(cv::Mat &src, cv::Mat &dst, double angle, const double scale=1.0);
};

}
#endif /* IMAGEROTATOR_H_ */

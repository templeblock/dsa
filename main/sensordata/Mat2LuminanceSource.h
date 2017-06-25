/*
 * Mat2LuminanceSource.h
 *
 *	Make a cv::Mat available to zxing library
 *
 *  Created on: 24.07.2013
 *      Author: jung
 */

#ifndef MAT2LUMINANCESOURCE_H_
#define MAT2LUMINANCESOURCE_H_

#include <opencv2/opencv.hpp>
#include "zxing/common/GreyscaleLuminanceSource.h"
#include "zxing/common/Array.h"

namespace dsa {

class Mat2LuminanceSource : public zxing::GreyscaleLuminanceSource {

	zxing::ArrayRef<char> mat2ArrayRef(cv::Mat &image);

public:
	Mat2LuminanceSource(cv::Mat &image);
	virtual ~Mat2LuminanceSource();
};

}
#endif /* MAT2LUMINANCESOURCE_H_ */

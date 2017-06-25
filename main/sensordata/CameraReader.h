/*
 * CameraReader.h
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#ifndef CAMERAREADER_H_
#define CAMERAREADER_H_

#include <string>
#include <generic/types.h>
#include <opencv2/opencv.hpp>

namespace dsa {

class CameraReader {
	cv::VideoCapture capture_;
	cv::Mat currentFrame_;
	UINT64 index_;	// the number of different frames captured by the reader (buffered deliveries via getCurrentFrame not included)

public:
	CameraReader(cv::VideoCapture* const capture);
	virtual ~CameraReader();

	cv::Mat getCurrentFrame();
	cv::Mat getNextFrame();

	UINT64 getCurrentFrameIndex();
	UINT64 getNextFrameIndex();
};

}
#endif /* CAMERAREADER_H_ */

/*
 * VideoReader.h
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#ifndef VIDEOREADER_H_
#define VIDEOREADER_H_

#include <string>
#include <generic/types.h>
#include <opencv2/opencv.hpp>

namespace dsa {

class VideoReader {
	std::string filename_;
	cv::VideoCapture capture_;
	cv::Mat currentFrame_;

public:
	VideoReader(const std::string &filename, const int startFrameIndex=0);
	VideoReader(const std::string &filename, const double positionRatio);
	virtual ~VideoReader();

	bool open(const std::string &filename="", const int startFrameIndex=0);
	bool isOpened() const;
	void close();

	cv::Mat getCurrentFrame();
	cv::Mat getNextFrame();

	std::string getFilename();
	void setFilename(const std::string &filename);
	int getCurrentFrameIndex();
	int getNextFrameIndex();
	bool setFrameIndex(const UINT32 frameIndex);
	bool setPositionRatio(const double positionRatio);
};

}
#endif /* VIDEOREADER_H_ */

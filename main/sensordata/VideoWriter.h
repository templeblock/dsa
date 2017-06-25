/*
 * VideoWriter.h
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#ifndef VIDEOWRITER_H_
#define VIDEOWRITER_H_

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
namespace dsa {

class VideoWriter {

	bool isColor_;
	bool initialized_;
	int fourcc_;
	double fps_;
	cv::Size frameSize_;

	std::string filename_;
	cv::VideoWriter writer_;
	cv::Mat currentFrame_;

	void init();

public:
	VideoWriter(const std::string &filename="output.avi", int fourcc=CV_FOURCC('F','L','V','1'), double fps=25.0, bool isColor=true);
	virtual ~VideoWriter();

	bool write();
	bool write(cv::Mat &image);

	std::string getFilename() const;
	cv::Mat getImage() const;
	void setImage(cv::Mat &image);
};

}
#endif /* VIDEOWRITER_H_ */

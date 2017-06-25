/*
 * ImageReader.h
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#ifndef IMAGEREADER_H_
#define IMAGEREADER_H_

#include <string>
#include <opencv2/opencv.hpp>

namespace dsa {

class ImageReader {
	int flags_;
	std::string filename_;
	cv::Mat image_;

public:
	ImageReader(const std::string &filename, const int flags=1);
	virtual ~ImageReader();

	bool open(const std::string &filename="", const int flags=1, bool force=false);
	bool openAsIs(const std::string &filename = "", bool force=false);
	bool openGrayscale(const std::string &filename = "", bool force=false);
	bool openRGB(const std::string &filename = "", bool force=false);
	bool isOpened() const;

	cv::Mat getImage();
	void close();

	std::string getFilename() const;
	void setFilename(const std::string &filename);
	int getFlags() const;
	void setFlags(const int flags);
};

}
#endif /* IMAGEREADER_H_ */

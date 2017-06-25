/*
 * ImageWriter.h
 *
 *  Created on: 10.09.2012
 *      Author: jung
 */

#ifndef IMAGEWRITER_H_
#define IMAGEWRITER_H_

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace dsa {

class ImageWriter {
	int jpegQuality_;
	int pngCompLevel_;
	bool pxmIsBinary_;

	std::string filename_;
	cv::Mat image_;

	void init();
	std::vector<int> getParams();

public:
	ImageWriter(const std::string &filename="");
	ImageWriter(cv::Mat &image, const std::string &filename="");
	virtual ~ImageWriter();

	bool write();
	bool write(const std::string filename);
	bool write(cv::Mat &image, const std::string &filename="");

	std::string getFilename();
	void setFilename(const std::string &filename);
	cv::Mat getImage();
	void setImage(cv::Mat &image);

	// JPEG (lossy) compression quality
	int getJpegQuality() const;
	void setJpegQuality(const int percent);

	// PNG (lossless) compression level
	int getPngCompression() const;
	void setPngCompression(const int percent);

	// PXM binary setting (see PPM, PGM, PBM image compression codec)
	bool getPxmBinary() const;
	void setPxmBinary(bool isBinary);
};

}
#endif /* IMAGEWRITER_H_ */

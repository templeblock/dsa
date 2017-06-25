/*
 * ImageBuffer.h
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef IMAGEBUFFER_H_
#define IMAGEBUFFER_H_

#include <list>
#include <generic/types.h>
#include <opencv2/opencv.hpp>

namespace dsa {

class ImageBuffer {

	static const size_t defaultBufferSize = 10;
	size_t size_;
	std::list<cv::Mat*> buffer_;

public:

	ImageBuffer(const size_t size=defaultBufferSize);
	virtual ~ImageBuffer();

	void write(cv::Mat& element);
	const cv::Mat* read() const;
	const cv::Mat* read(const size_t n) const;
	const cv::Mat* readFirst() const;
	std::vector<cv::Mat*>* readLastN(const size_t nElements) const;
	void free();
	void resize(const size_t size);
	bool isEmpty() const;
	bool isFull() const;
	size_t getSize() const;

};

}
#endif /* IMAGEBUFFER_H_ */

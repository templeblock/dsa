/*
 * TimeStampWriterFilter.cpp
 *
 *  Created on: 10.04.2013
 *      Author: Bigontina
 */

#include <TimeStampWriterFilter.h>
#include <opencv2/opencv.hpp>

dsa::TimeStampWriterFilter::TimeStampWriterFilter() {}

void dsa::TimeStampWriterFilter::init() {}

void dsa::TimeStampWriterFilter::apply() {
	if(isReady()) {
		dst = cv::Mat::zeros(src->rows+20, src->cols, src->type());
		src->copyTo(dst.rowRange(0,src->rows));

		std::time_t result = std::time(NULL);
		std::string s(std::asctime(std::localtime(&result)));

		cv::putText(dst, s, cv::Point(10,dst.rows-2), 1, 1, cv::Scalar(255,255,255),1);
	}

}

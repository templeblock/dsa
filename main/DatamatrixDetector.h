/*
 * DatamatrixDetector.h
 *
 *  Created on: 27.03.2012
 *      Author: jung
 */

#ifndef DATAMATRIXDETECTOR_H_
#define DATAMATRIXDETECTOR_H_

#include <generic/helper.h>

namespace dsa {

class DatamatrixDetector {
protected:
	cv::Mat image_;

	int computeBlackWhiteChanges(cv::Mat& mat);
	float computeAngle(Square& square);
	const cv::Rect computeInnerDataMatrixRectangle(cv::Mat& mat, float threshold);

public:
	DatamatrixDetector(const cv::Mat& mat);
	virtual ~DatamatrixDetector();

	const cv::Point find(SquareList &squareList);
};

}
#endif /* DATAMATRIXDETECTOR_H_ */

/*
 * BarcodeSegmenter.cpp
 *
 * 	See
 *
 *	barcode_extraction_framework.pdf
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef BARCODESEGMENTER_H_
#define BARCODESEGMENTER_H_

#include <generic/helper.h>
#include <generic/ImageRotator.h>

namespace dsa {

class BarcodeSegmenter {

	int paper_prefilter_threshold_;
	int paper_prefilter_operations_ksize_;

	int minLong_;
	int minShort_;
	int minSize_;

	int darkThreshold_;	// what is considered a dark color
	int lightThreshold_; // what is considered a light color
	double maxMediumRatio_;	// how big do we allow the remaining region to get

	// determine how close to a feasible rectangle this segment is
	double maxPerimeterRatio_; // how close is the perimeter to that of our rotated rectangle?
	double minAreaCoverage_;	// how close is the area to that of our rotated rectangle?
	double maxSideLengthRatio_;	// do length and width suggest a feasible rectangle for a barcode

	int ksize_;
	int windowSize_;

	void init();
	void prefilter(cv::Mat &src, cv::Mat &dst);
	void label(cv::Mat &src, cv::Mat &mask, std::vector<cv::Point> &min, std::vector<cv::Point> &max, std::vector<int> &size);
	void segmentsFromComponents(cv::Mat &image, std::vector<Roi> &segments, cv::Mat &smask, std::vector<cv::Point> &min, std::vector<cv::Point> &max, std::vector<int> &size);
	void enhanceComponent(cv::Mat &image, cv::Mat &mask, int compSize);

public:
	BarcodeSegmenter(const int ksize=8);
	virtual ~BarcodeSegmenter();

	void find(cv::Mat &image, std::vector<Roi> &segments);
};

}
#endif /* BARCODESEGMENTER_H_ */

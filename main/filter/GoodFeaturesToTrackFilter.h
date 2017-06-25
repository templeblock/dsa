/*
 * CornerSubPixFilter.h
 *
 *	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#goodfeaturestotrack
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#ifndef GOODFEATURESTOTRACKFILTER_H_
#define GOODFEATURESTOTRACKFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class GoodFeaturesToTrackFilter : public EvaluationFilter {

	cv::Mat corners_;
	int maxCorners_;
	double qualityLevel_;
	double minDistance_;
	cv::Mat mask_;
	int blockSize_;
	bool useHarrisDetector_;
	double k_;

	void init();
	void evaluate();

public:

	cv::Mat* corners;
	int* maxCorners;
	double* qualityLevel;
	double* minDistance;
	cv::Mat* mask;
	int* blockSize;
	bool* useHarrisDetector;
	double* k;

	GoodFeaturesToTrackFilter(cv::Mat& corners, int maxCorners, double qualityLevel, double minDistance, const cv::Mat& mask=cv::Mat(), int blockSize=3, bool useHarrisDetector=false, double k=0.04);
	virtual ~GoodFeaturesToTrackFilter();

};

}
#endif /* GOODFEATURESTOTRACKFILTER_H_ */

/*
 * GoodFeaturesToTrackFilter.cpp
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

#include "GoodFeaturesToTrackFilter.h"

/**
 * Constructor
 *
 * @param corners: Output vector of detected corners.
 * @param maxCorners: Maximum number of corners to return. If there are more corners than are found, the strongest of them is returned.
 * @param qualityLevel: Parameter characterizing the minimal accepted quality of image corners. The parameter value is multiplied by the best corner quality measure, which is the minimal eigenvalue or the Harris function response.
 * @param minDistance: Minimum possible Euclidean distance between the returned corners.
 * @param mask: Optional region of interest. If the image is not empty (it needs to have the type CV_8UC1 and the same size as image), it specifies the region in which the corners are detected.
 * @param blockSize: Size of an average block for computing a derivative covariation matrix over each pixel neighborhood.
 * @param useHarrisDetector: Parameter indicating whether to use a Harris detector.
 * @param k: Free parameter of the Harris detector.
 */
dsa::GoodFeaturesToTrackFilter::GoodFeaturesToTrackFilter(cv::Mat& corners, int maxCorners, double qualityLevel, double minDistance, const cv::Mat& mask, int blockSize, bool useHarrisDetector, double k) {
	corners_ = corners;
	maxCorners_ = maxCorners;
	qualityLevel_ = qualityLevel;
	minDistance_ = minDistance;
	mask_ = mask;
	blockSize_ = blockSize;
	useHarrisDetector_ = useHarrisDetector;
	k_ = k;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::GoodFeaturesToTrackFilter::~GoodFeaturesToTrackFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::GoodFeaturesToTrackFilter::init() {
	corners = &corners_;
	maxCorners = &maxCorners_;
	qualityLevel = &qualityLevel_;
	minDistance = &minDistance_;
	mask = &mask_;
	blockSize = &blockSize_;
	useHarrisDetector = &useHarrisDetector_;
	k = &k_;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::GoodFeaturesToTrackFilter::evaluate() {
	goodFeaturesToTrack(*src, *corners, *maxCorners, *qualityLevel, *minDistance, *mask, *blockSize, *useHarrisDetector, *k);
}

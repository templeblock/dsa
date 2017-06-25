/*
 * ForkAllocationSensorInitializer.h
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#ifndef FORKALLOCATIONSENSORINITIALIZER_H_
#define FORKALLOCATIONSENSORINITIALIZER_H_

#include <set>
#include <string>
#include "Sensorfunction.h"
#include <generic/helper.h>
#include <sensordata/VideoHub.h>
#include <filter/FilterChain.h>
#include <filter/CvtColorFilter.h>
#include <filter/ResizeFilter.h>
#include <filter/GrabImageFilter.h>
#include <filter/SubtractMeanFilter.h>
#include <filter/ForkBackgroundFilter.h>
#include <filter/RotationFilter.h>
#include <filter/ImageOutputFilter.h>
#include <filter/WaitFilter.h>

namespace dsa {

class ForkAllocationSensorInitializer : public Sensorfunction {
	bool useConfig_;
	bool skipDetermineBinary_; // configure run() method: false: run determineBinaryImage() in a loop, true: run externally determineBinaryImage(Mat) via external calls
	int runs_;
	VideoHub* hub_;

	cv::Mat binaryImage_;
	cv::Mat forkReferenceImage_;
	FilterChain *filterChain_;

	std::vector<cv::Rect> rois_;
	std::vector<std::vector<cv::Point> > forks_;
	std::vector<cv::Mat> forkMasks_;
	std::vector<cv::Mat> forkReferences_;
	std::vector<cv::Mat> forkHistograms_;
	std::vector<std::vector<size_t> *> maskedPixelsPerLine_;
	std::string configFile_;

	void init();
	void determineBinaryImage();
	void findForks();

public:

	ForkAllocationSensorInitializer(VideoHub* dataHub, std::string &configFile);
	ForkAllocationSensorInitializer();
	ForkAllocationSensorInitializer(std::string &configFile);
	virtual ~ForkAllocationSensorInitializer();

	bool calibrate();
	void run();

	bool isReady();

	void determineBinaryImage(cv::Mat *input);
	cv::Mat getBinaryImage();
	void reset();
	void setConfig(std::string &configFile);

	std::vector<cv::Rect>& getROIs();
	std::vector<std::vector<cv::Point> >& getForks();
	std::vector<cv::Mat>& getMasks();
	std::vector<cv::Mat>& getReferences();
	std::vector<cv::Mat>& getHistograms();

	std::vector<std::vector<size_t> *>& getMaskedPixelsPerLine();

	bool writeToXML(const std::string &filename) const;
	bool readFromXML(const std::string &filename);
};

}
#endif /* FORKALLOCATIONSENSORINITIALIZER_H_ */

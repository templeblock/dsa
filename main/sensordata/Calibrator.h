/*
 * Calibrator.h
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#ifndef CALIBRATOR_H_
#define CALIBRATOR_H_

#include <ctime>
#include <generic/types.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#ifdef WITH_RASPICAM
	#include <raspicam_cv.h>
	#include <unistd.h>
#endif
namespace dsa {

class Calibrator {

	static const int MAX_VIEW_WIDTH = 640;

	UINT32 calibrations_;
	UINT32 ncornersX_;
	UINT32 ncornersY_;

	cv::VideoCapture* capture_cv_;	// either from camera feed, 
	raspicam::RaspiCam_Cv* capture_;
	std::vector<cv::Mat*> images_;	// or from a set of images

	CalibrationParameters parameters_;

public:
	Calibrator(cv::VideoCapture* const capture, const UINT32 calibrations=10, const UINT32 ncornersX=9, const UINT32 nconersY=6);
	Calibrator(raspicam::RaspiCam_Cv* const capture, const UINT32 calibrations=10, const UINT32 ncornersX=9, const UINT32 nconersY=6);
	Calibrator(std::vector<cv::Mat*> images);
	virtual ~Calibrator();

	bool run();
	bool writeCalibrationToXML(const std::string &filename) const;
	bool readCalibrationFromXML(const std::string &filename);
	const CalibrationParameters getCalibrationParams() const;
};

}
#endif /* CALIBRATOR_H_ */

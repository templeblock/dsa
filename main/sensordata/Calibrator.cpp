/*
 * Calibrator.cpp
 *
 *  Created on: 04.07.2012
 *      Author: jung
 */

#include "Calibrator.h"

dsa::Calibrator::Calibrator(cv::VideoCapture* const capture, const UINT32 calibrations,  const UINT32 ncornersX, const UINT32 ncornersY) {
	capture_cv_= capture;
	calibrations_ = calibrations;
	ncornersX_ = ncornersX;
	ncornersY_ = ncornersY;

	parameters_.cameraMatrix = cv::Mat(3, 3, CV_32FC1);
	parameters_.cameraMatrix.at<float>(0, 0) = 1;
	parameters_.cameraMatrix.at<float>(1, 1) = 1;
}

dsa::Calibrator::Calibrator(raspicam::RaspiCam_Cv* const capture, const UINT32 calibrations,  const UINT32 ncornersX, const UINT32 ncornersY) {
	capture_= capture;
	calibrations_ = calibrations;
	ncornersX_ = ncornersX;
	ncornersY_ = ncornersY;

	parameters_.cameraMatrix = cv::Mat(3, 3, CV_32FC1);
	parameters_.cameraMatrix.at<float>(0, 0) = 1;
	parameters_.cameraMatrix.at<float>(1, 1) = 1;
}

dsa::Calibrator::~Calibrator() {
	// TODO Auto-generated destructor stub
}

bool dsa::Calibrator::run() {

   capture_->set(CV_CAP_PROP_FRAME_WIDTH, 640);
   capture_->set(CV_CAP_PROP_FRAME_HEIGHT, 480);
   capture_->set(CV_CAP_PROP_FPS, 25.0);
   if(!capture_->open()){
				std::cout<<"Error opening camera"<<std::endl; return 1; 
	}
	else
		std::cout<<"opened RaspiCam_Cv OK, going sleep 3 secs"<<std::endl;

	const UINT32 fps = 15;
	cv::Size boardSize = cv::Size(ncornersX_, ncornersY_);

	std::vector<cv::Point2f> corners;
	std::vector<std::vector<cv::Point2f> > imageCoords;

	// create coordinates for each square
	std::vector<cv::Point3f> cornerCoords;
	UINT32 nsquares = ncornersX_ * ncornersY_;
	for(UINT32 i = 0; i < nsquares; i++)
		cornerCoords.push_back(cv::Point3f(i/ncornersX_, i%ncornersX_, 0.0f));
	// add those coordinates to each calibration we'll do
	std::vector<std::vector<cv::Point3f> > objectCoords(calibrations_, cornerCoords);

	cv::Mat image;
	cv::namedWindow("Calibration", CV_WINDOW_AUTOSIZE);
   int index = 0;
	// for all calibration runs
	for(UINT32 calibrations = 0; calibrations < calibrations_; ) {
		clock_t start = clock();
		//capture_->read(image);
      std::cout<< "Start frame "<< index << std::endl;
		capture_->grab();
		capture_->retrieve(image);
		std::cout<< "empty? "<< image.empty() <<" calibration "<< calibrations << std::endl;
		// convert to grayscale if not already (currently only works on RGB images)
		//cv::Mat image2 = image.clone();
		if(image.channels() > 1)
			cv::cvtColor(image, image, CV_RGB2GRAY);
		//cv::resize(image2, image, cv::Size(1224, 1028));



		cv::Mat smallImage;
		cv::Mat *pImage;
		/* to speed things up with huge images, find the chessboard corners on a smaller scale image first, have the user confirm
		 * and then find it again on the original image. This way we only need to run this task only on the full frames we really
		 *  want to use */
		bool usedResized = (image.cols > MAX_VIEW_WIDTH);
		if(usedResized) {
			double scaleFactor = (double)MAX_VIEW_WIDTH/image.cols;
			cv::resize(image, smallImage, cv::Size(0,0), scaleFactor, scaleFactor);
			pImage = &smallImage;
		} else
			pImage = &image;

		/* Most expensive Operation on the small image */
		const bool found = cv::findChessboardCorners(*pImage, boardSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FILTER_QUADS);
		if(found)
			drawChessboardCorners(*pImage, boardSize, corners, found);
		corners.clear();

		imshow("Calibration", *pImage);
      cv::waitKey(2000);
    
		// wait, to not exceed framerate
		int waitFor = CLOCKS_PER_SEC/fps - (int)(clock() - start);
		int key = (waitFor > 0) ? cv::waitKey(waitFor) : cv::waitKey(1);
		// evaluate keyboard input
		if(key == 'q')
			return(false);	// quit
		if(found) {
			std::cout<<"first found true"<<std::endl;
			// we have to do it again on the original image, if we used the resized image above
			if(!usedResized || cv::findChessboardCorners(image, boardSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FILTER_QUADS)) {
				// determine more accurate position
				cv::cornerSubPix(image, corners, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 30, 0.01));
				// save the determined coords
				imageCoords.push_back(corners);

				calibrations++;
			}
		}
   
      index++;
	}

	std::vector<cv::Mat> rvecs, tvecs;
	// determine camera parameters
	cv::calibrateCamera(objectCoords, imageCoords, image.size(), parameters_.cameraMatrix, parameters_.distCoeffs, rvecs, tvecs);
/*
   cv::InputArrayOfArrays a = objectCoords;
   cv::InputArrayOfArrays b = imageCoords;
   cv::calibrateCamera(a, b, image.size(), parameters_.cameraMatrix, parameters_.distCoeffs, rvecs, tvecs);*/

	capture_->release();
	return(true);
}

bool dsa::Calibrator::writeCalibrationToXML(const std::string &filename) const {
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	bool success = fs.isOpened();
	if(success) {
		fs << "cameraMatrix" << parameters_.cameraMatrix;
		fs << "distCoeffs" << parameters_.distCoeffs;
	}
	fs.release();

	return(success);
}

bool dsa::Calibrator::readCalibrationFromXML(const std::string &filename) {
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	bool success = fs.isOpened();
	if(success) {
		fs["cameraMatrix"] >> parameters_.cameraMatrix;
		fs["distCoeffs"] >> parameters_.distCoeffs;
	}
	fs.release();

	return(success);
}

const dsa::CalibrationParameters dsa::Calibrator::getCalibrationParams() const {
	return(CalibrationParameters(parameters_));
}

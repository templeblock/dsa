/*
 * CoordinateTransformFilter.h
 *
 *	Performs a coordinate transformation.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "CoordinateTransformFilter.h"

/**
 * Constructor
 */
dsa::CoordinateTransformFilter::CoordinateTransformFilter() {
	useTranslationOffset_ = false;
	useRotationOffset_ = false;
	translationTransformer_ = cv::Mat();
	rotationTransformer_ = cv::Mat();
	position = 0;
	rotation = 0;
	transPosition = cv::Mat::zeros(3,1,CV_32FC1);
	transRotation = cv::Mat::zeros(3,1,CV_32FC1);

	init();
}

/**
 * Constructor
 */
dsa::CoordinateTransformFilter::CoordinateTransformFilter(cv::Mat &translationTransformer, cv::Mat &rotationTransformer, bool useTranslationOffset, bool useRotationOffset) {
	translationTransformer_ = translationTransformer;
	rotationTransformer_ = rotationTransformer;
	useTranslationOffset_ = useTranslationOffset;
	useRotationOffset_ = useRotationOffset;
	position = 0;
	rotation = 0;
	transPosition = cv::Mat::zeros(3,1,CV_32FC1);
	transRotation = cv::Mat::zeros(3,1,CV_32FC1);

	init(); 
}

/**
 * Destructor
 */
dsa::CoordinateTransformFilter::~CoordinateTransformFilter() {
}

void dsa::CoordinateTransformFilter::init() {
	translationTransformer = &(translationTransformer_);
	rotationTransformer = &(rotationTransformer_);
	//translationTransformer_ = *translationTransformer;
	//rotationTransformer_ = *rotationTransformer;
}

void dsa::CoordinateTransformFilter::preprocessing() {
	EvaluationFilter::preprocessing();
}

void dsa::CoordinateTransformFilter::postprocessing() {
	EvaluationFilter::postprocessing();
}

/**
 * Fill internal data structure with markers
 */
void dsa::CoordinateTransformFilter::evaluate() {
	for(size_t i = 0; i < 3; i++) {
		transPosition.at<float>(i) = position->at<float>(i);
		transRotation.at<float>(i) = rotation->at<float>(i);
	}
	//std::cerr << "vorher " << transRotation.at<float>(2)*180.f/PI << std::endl;
	//transRotation.at<float>(2) = mapAngle(transRotation.at<float>(2) + rotationTransformer_.at<float>(2));
	//std::cerr << "nacher" << transRotation.at<float>(2)*180.f/PI << std::endl;
	//return;
	// first translate from camera coordinates to the center of the vehicle coordinate system
	cv::Mat transOffset = cv::Mat::zeros(3, 1, CV_32FC1);
	cv::Mat rotOffset = cv::Mat::zeros(3, 1, CV_32FC1);
	if(useTranslationOffset_) {
		for(size_t i = 0; i < 3; i++)
			transOffset.at<float>(i) = translationTransformer_.at<float>(i);
	}
	if(useRotationOffset_) {
		for(size_t i = 0; i < 3; i++)
			rotOffset.at<float>(i) = rotationTransformer_.at<float>(i); 
	}
	// apply translation first
	for(size_t i = 0; i < 3; i++) {
		//std::cout << "1transPosition" << i << "=" << transPosition.at<float>(i) << std::endl;
		transPosition.at<float>(i) = position->at<float>(i) + translationTransformer->at<float>(i) + transOffset.at<float>(i);
		//std::cout << "2transPosition" << i << "=" << transPosition.at<float>(i) << std::endl;
	}

	float sinAngle;
	float cosAngle;
	float x, y, z;
	// now apply rotation
	// x-axis:
	if(rotationTransformer->at<float>(0) != 0.f) {
		float angle = mapAngle(rotationTransformer->at<float>(0) + rotOffset.at<float>(0));
		sinAngle = sin(angle);
		cosAngle = cos(angle);
		x = transPosition.at<float>(0), y = transPosition.at<float>(1), z = transPosition.at<float>(2);
		transPosition.at<float>(0) = x;
		transPosition.at<float>(1) = y*cosAngle + z*sinAngle;
		transPosition.at<float>(2) = -y*sinAngle + z*cosAngle;
	}
	// y-axis:
	if(rotationTransformer->at<float>(1) != 0.f) {
		float angle = mapAngle(rotationTransformer->at<float>(1) + rotOffset.at<float>(1));
		sinAngle = sin(angle);
		cosAngle = cos(angle);
		x = transPosition.at<float>(0), y = transPosition.at<float>(1), z = transPosition.at<float>(2);
		transPosition.at<float>(0) = x*cosAngle - z*sinAngle;
		transPosition.at<float>(1) = y;
		transPosition.at<float>(2) = x*sinAngle + z*cosAngle;
	}
	// z-axis:
	if(rotationTransformer->at<float>(2) != 0.f) {
		float angle = mapAngle(rotationTransformer->at<float>(2) + rotOffset.at<float>(2));
		sinAngle = sin(angle);
		cosAngle = cos(angle);
		x = transPosition.at<float>(0), y = transPosition.at<float>(1), z = transPosition.at<float>(2);
		transPosition.at<float>(0) = x*cosAngle + y*sinAngle;
		transPosition.at<float>(1) = -x*sinAngle + y*cosAngle;
		transPosition.at<float>(2) = z;
	}
}

/**
 * Map an angle in degrees to the interval [-180, 180[;
 *
 * @param angle
 * @return the mapped angle
 */
float dsa::CoordinateTransformFilter::mapAngle(float angle) {
	float result = angle;
	while(result >= PI)
		result -= 2*PI;
	while(result < -PI)
		result += 2*PI;
	return(result);
}

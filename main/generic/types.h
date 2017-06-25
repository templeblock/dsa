/*
 * types.h
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <list>
#include <string>
#include <algorithm>
#include <vector>
#include <opencv2/opencv.hpp>
#include "zxing/BarcodeFormat.h"

namespace dsa {

typedef char INT8;				// alias for char
typedef short INT16;			// short alias for unsigned short
typedef int INT32;				// alias for int
typedef long long INT64;		// short alias for long long
typedef unsigned char UINT8;	// short alias for unsigned char
typedef unsigned short UINT16;	// short alias for unsigned short
typedef unsigned int UINT32;	// short alias for unsigned int
typedef unsigned long long UINT64;	// short alias for unsigned long long

enum SensorType {
	Localization,	// determines vehicle pose
	ForkHeight,		// determines fork height and mast tilt angle
	ForkOccupied,	// fork occupied or not
	CodeReader,		// reads bar and 2d codes
	ForkCam,		// fork camera
	ForkDistance,	// estimates distance from fork to pallet (not implemented)
	ForkInsertionAssist,	// assists in properly inserting the forks into the pallet (not implemented)
	collisionAssist,		// detects objects the forklift is inclined to hit to avoid those collisions (not implemented)
	humanDetectionAssist	// detects people around the forklift to avoid collisions (not implemented)
};

enum NodeType { InNode, OutNode };
enum ForkState {
	idle,		// nicht belegt
	occupied,	// belegt
	unknown,	// unbekannt
	lost		// Demonstrator hat gabel verloren
};

enum MLType {
	NB,
	KNN,
	SVM,
	DT,
	BOOST,
	GBT,
	RT,
	ERT,
	EM,
	MLP,
};

/**
 *	Camera calibration parameters together in one object
 */
struct CalibrationParameters {
	cv::Mat cameraMatrix;	// intrinsic paramters
	cv::Mat distCoeffs;		// distance coefficients

	CalibrationParameters() {
	}

	CalibrationParameters(const CalibrationParameters& copy) {
		cameraMatrix = copy.cameraMatrix.clone();
		distCoeffs = copy.distCoeffs.clone();
	}
};

/**
 * A Square is a set of 4 CvPoints representing a polygonal line to define a convex polygon by connecting the points
 * in the following order: p[0]->p[1]->p[2]->p[3]->[0]
 */
struct Square {
	cv::Point p[4];
};

struct Circle {
	cv::Point center;
	int radius;
};

struct SquareList {
	std::list<Square*> squares;
	std::list<Square*>::const_iterator s_it;

	~SquareList() {
		while(! squares.empty()) {
			delete squares.back();
			squares.pop_back();
		}
	}
};

struct Roi {
	cv::RotatedRect rotatedRect;
	cv::Mat image;
};

struct Barcode {
	cv::RotatedRect rrect;
	zxing::BarcodeFormat::Value format;
	std::string contents;

	Barcode() {
		rrect = cv::RotatedRect();
		format = zxing::BarcodeFormat::Value::NONE;
		contents = std::string("");
	}

	Barcode(const Barcode& rhs) {
		rrect = rhs.rrect;
		format = rhs.format;
		contents = std::string(rhs.contents);
	}

	Barcode(cv::RotatedRect rotatedRect, zxing::BarcodeFormat::Value barcodeFormat, std::string string) {
		rrect = rotatedRect;
		format = barcodeFormat;
		contents = std::string(string);
	}
};

struct BarcodeEvent {
	int timestamp;
	Barcode barcode;

	BarcodeEvent(int timestamp, Barcode &barcode) {
		this->timestamp = timestamp;
		this->barcode = Barcode(barcode);
	}

	BarcodeEvent(const BarcodeEvent& rhs) {
		timestamp = rhs.timestamp;
		barcode = Barcode(rhs.barcode);
	}
};

struct GenericMLResult {
	MLType type;
	cv::Mat vresult;

	GenericMLResult(MLType type, cv::Mat vresult) {
		this->type = type;
		this->vresult = vresult;
	}
};

struct BinaryMLResult {
	MLType type;
	bool isObject;

	BinaryMLResult(MLType type, bool isObject) {
		this->type = type;
		this->isObject = isObject;
	}
};

} /* namespace dsa */

#endif /* TYPES_H_ */

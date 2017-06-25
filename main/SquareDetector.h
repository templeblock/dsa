/*
 * SquareDetector.h
 *
 *  Created on: 27.03.2012
 *      Author: jung
 */

#ifndef SQUAREDETECTOR_H_
#define SQUAREDETECTOR_H_

#include <vector>
#include <generic/definitions.h>
#include <generic/helper.h>

// definitions for square detector
static const int DEFAULT_LENGTH_THRESHOLD = 15;
static const float DEFAULT_OPPOSING_THRESHOLD = 0.4f;
static const float DEFAULT_ADJACENT_THRESHOLD = 0.6f;
static const float DEFAULT_COSINE_THRESHOLD = 0.98f;

namespace dsa {

class SquareDetector {
private:
	cv::Mat image_;
	int lengthThreshold_;	// minimal length for all lines of a square to be considered
	float opposingThreshold_; // max difference in length (opposing edges)
	float adjacentThreshold_; // max difference between length (adjacent edges)
	float cosineThreshold_;	// opposing edges shall not be oriented towards each other at an angle who's cosine smaller this value

public:
	// constructor
	SquareDetector(const cv::Mat& mat, const int lengthThreshold=DEFAULT_LENGTH_THRESHOLD, const float opposingThreshold=DEFAULT_OPPOSING_THRESHOLD, const float adjacentThreshold=DEFAULT_ADJACENT_THRESHOLD, const float cosineThreshold=DEFAULT_COSINE_THRESHOLD);

	// destructor
	virtual ~SquareDetector();

	SquareList* find();
};

}
#endif /* SQUAREDETECTOR_H_ */

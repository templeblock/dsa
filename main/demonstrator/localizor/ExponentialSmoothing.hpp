/*
 * ExponentialSmoothing.hpp
 *
 *  Created on: 12.03.2013
 *      Author: Bigontina
 */

#ifndef EXPONENTIALSMOOTHING_HPP_
#define EXPONENTIALSMOOTHING_HPP_

#include <opencv2/opencv.hpp>

#include <RotationCalcs.hpp>

using namespace cv;

/*
 * Applies exponential smoothing to matrices as input (component-wise).
 * Rotations are a special case and have to be declared as such.
 */
class ExponentialSmoothing {

public:
	Mat smoothed;
	float alpha;
	bool isRotation;
	bool initialized;

	ExponentialSmoothing() {}

	ExponentialSmoothing(float alpha, bool isRotation=false) {
		this->alpha = alpha;
		this->isRotation = isRotation;
		initialized = false;
	}

	void add(Mat val) {
		if(!initialized) {
			val.copyTo(smoothed);
			initialized = true;
		} else {
			if(isRotation) rearrangeRotations(val, smoothed);
			smoothed = val * alpha + smoothed * (1.0-alpha);
			if(isRotation) normalizeRotation(smoothed);
		}
	}

	void get(Mat smoothed) {
		this->smoothed.copyTo(smoothed);
	}

};


#endif /* EXPONENTIALSMOOTHING_HPP_ */

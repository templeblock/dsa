/*
 * MarkerInfo.hpp
 *
 *  Created on: Jan 8, 2013
 *      Author: Bigontina
 */

#ifndef MARKERINFO_HPP_
#define MARKERINFO_HPP_

#include "opencv2/opencv.hpp"

using namespace cv;

/*
 * Stores information about a marker, i.e. the absolute position
 * in the warehouse, its rotation, its size and its id.
 * Instances of this class a created by MarkerStorage.
 */
class MarkerPose {
public:
	int id;
	Mat position, rotation;
	float size;

	MarkerPose(int id, Mat position, Mat rotation, float size) {
		this->id = id;
		this->position = position;
		this->rotation = rotation;
		this->size = size;
	}

	void getPosition(Mat & p) {
		position.copyTo(p);
	}

	void getRotation(Mat & r) {
		rotation.copyTo(r);
	}

	void get(Mat & p, Mat & r) {
		getPosition(p);
		getRotation(r);
	}

	MarkerPose copy() {
		Mat p, r;
		position.copyTo(p);
		rotation.copyTo(r);
		MarkerPose pose = MarkerPose(id, p, r, size);
		return(pose);
	}

};


#endif /* MARKERINFO_HPP_ */

/*
 * SimulatePoseFilter.h
 *
 *	This filter simulates localization information for testing purposes.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef SIMULATEPOSEFILTER_H_
#define SIMULATEPOSEFILTER_H_

#include <cmath>
#include <generic/types.h>
#include <generic/helper.h>
#include "EvaluationFilter.h"
#include <demonstrator/localizor/ExponentialSmoothing.hpp>

namespace dsa {

class SimulatePoseFilter : public EvaluationFilter {

	int x, y;
	float r;
	int transCoords_;
	int rotCoords_;

	void init();
	void evaluate();

public:

	float tiltAngle;
	int forkHeight;
	int forkOccupied1;
	int forkOccupied2;

	clock_t timestamp;
	cv::Mat camPosition;
	cv::Mat camRotation;

	SimulatePoseFilter(int transCoords=3, int rotCoords=4);	// sum of the coordinates used for translation and rotation: x=1, y=2, z=4. default values are for localization in x,y plane
	virtual ~SimulatePoseFilter();

	bool isReady() const;
	void preprocessing();
};

}
#endif /* SIMULATEPOSEFILTER_H_ */

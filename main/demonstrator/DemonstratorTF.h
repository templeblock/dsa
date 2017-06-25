/*
 * DemonstratorTF.h
 *
 *  Created on: 19.04.2012
 *      Author: Jung
 */

#ifndef DEMONSTRATORTF_H_
#define DEMONSTRATORTF_H_

#include <iostream>
#include <ctime>
#include "SquareDetector.h"
#include "DatamatrixDetector.h"
#include <generic/VectorPredictor.h>
//#include <generic/StaticPredictor.h>
//#include <generic/ExpFilterPredictor.h>

namespace dsa {

class DemonstratorTF {
public:
	DemonstratorTF(const UINT32 camera, const UINT32 predictionRadius=20);
	virtual ~DemonstratorTF();

	int run();	// execute the demonstrator

private:
	static const UINT32 framesPerSecond = 25;
	static const UINT32 msPerFrame = CLOCKS_PER_SEC/framesPerSecond;
	int predictionRadius; // detected point within circle with this radius => hit, otherwise miss

	cv::VideoCapture capture;
};

}
#endif /* DEMONSTRATORTF_H_ */

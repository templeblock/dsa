/*
 * LocalizationToDatagramFilter.cpp
 *
 * Export localization information via a datagram
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef LOCALIZATIONTODATAGRAMFILTER_H_
#define LOCALIZATIONTODATAGRAMFILTER_H_

#include <vector>
#include <boost/array.hpp>
#include <generic/helper.h>
#include "EvaluationFilter.h"

namespace dsa {

class LocalizationToDatagramFilter : public EvaluationFilter {

	int forkOccupied1_;
	int forkOccupied2_;
	int packetType_;
	int vehicleID_;

	float forkHeight_;
	float forkRotation_;

	void init();
	void preprocessing();
	void evaluate();

public:

	int *forkOccupied1;
	int *forkOccupied2;
	clock_t* timestamp;
	cv::Mat* camPosition;
	cv::Mat* camRotation;
	float* forkHeight;
	float* forkRotation;
	std::vector<char> data;

	LocalizationToDatagramFilter(int vehicleID);
	virtual ~LocalizationToDatagramFilter();
};

}
#endif /* LOCALIZATIONTODATAGRAMFILTER_H_ */

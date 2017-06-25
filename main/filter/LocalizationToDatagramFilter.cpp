/*
 * LocalizationToDatagramFilter.cpp
 *
 * Export localization information via a datagram
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "LocalizationToDatagramFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::LocalizationToDatagramFilter::LocalizationToDatagramFilter(int vehicleID) {
	vehicleID_ = vehicleID;
	packetType_ = 0;
	forkHeight_ = 0;
	forkRotation_ = 0;
	forkOccupied1_ = 0;
	forkOccupied2_ = 0;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::LocalizationToDatagramFilter::~LocalizationToDatagramFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::LocalizationToDatagramFilter::init() {
	forkOccupied1 = &(forkOccupied1_);
	forkOccupied2 = &(forkOccupied2_);
	forkHeight = &(forkHeight_);
	forkRotation = &(forkRotation_);
	data = std::vector<char>();
}

void dsa::LocalizationToDatagramFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	data.clear();
}

/**
 * The payload of the packet always contains the following information in the given order:
 *
 * 1. packet type as char: 1 Byte (currently we only have one packet type which is "0" and contains the fork status
 * 2. vehicle id as char: 1 Byte (allows for up to 256 vehicles)
 * 3. timestamp as int: 4 Bytes (milliseconds since beginning of this day)
 * 4. translation coordinate t1 as int: 4 Bytes (in cm)
 * 5. translation coordinate t2 as int: 4 Bytes (in cm)
 * 6. translation coordinate t3 as int: 4 Bytes (in cm)
 * 7. rotation angle r1 as short: 2 Bytes (in 1/100 degrees)
 * 8. rotation angle r2 as short: 2 Bytes (in 1/100 degrees)
 * 9. rotation angle r3 as short: 2 Bytes (in 1/100 degrees)
 * 10. fork height as short: 2 Bytes (in mm => max height is currently ~16,3m, which should be enough for now)
 * 11. mast tilt angle as short: 2 Bytes (in 1/100 degrees)
 * 12. fork allocation status of first fork set as char: 1 Byte (in percent, we only use a boolean state ourselves, therefore 100 means occupied, 0 means not occupied)
 * 13. fork allocation status of second fork set as char: 1 Byte (in percent, we do not use this at all, therefore the values transmitted but empty)
 *
 * => total 30 Bytes
 *
 * in case the vehicle does not support a sensor it shall simply set the not available information to zero. it is up to the recipient to make sense of it.
 */
void dsa::LocalizationToDatagramFilter::evaluate() {
	if(camPosition->at<float>(0,0) != -FLT_MAX) {
		// packet type 1 Byte
		data.push_back(signedChar(packetType_));
		// vehicle id 1 Byte (allows for up to 256 vehicles)
		data.push_back(signedChar(vehicleID_));
		// timestamp (milliseconds since beginning of this day)
		boost::array<char, 4> temp = splitWordToBytes((int)(*timestamp));
		for(size_t i = 0; i < 4; i++)
			data.push_back(temp[i]);
		// translation(t1, t2, t3)  4 Bytes each (in cm)
		for(size_t i = 0; i < 3; i++) {
			temp = splitWordToBytes((int)(10*camPosition->at<float>(0, i)));
			for(size_t j = 0; j < 4; j++)
				data.push_back(temp[j]);
		}
		// rotation(r1, r2, r3) 2 Bytes each (in 1/100 degrees)
		for(size_t i = 0; i < 3; i++) {
			boost::array<char, 2> temp2 = splitHalfwordToBytes((short)(100.0f*180.0f*camRotation->at<float>(0, i)/PI));
			for(size_t j = 0; j < 2; j++) {
				data.push_back(temp2[j]);
			}
		}

		// fork height 2 Bytes (in mm => max height is currently ~32,8m, which should be enough for now)
		boost::array<char, 2> temp2 = splitHalfwordToBytes(signedShort(10* *forkHeight));
		for(size_t j = 0; j < 2; j++) {
			data.push_back(temp2[j]);
		}

		// fork tilt angle 2 Bytes (in 1/100 degrees)
		temp2 = splitHalfwordToBytes((short)(100.0f*180.0f*(*forkRotation)/PI));
		for(size_t j = 0; j < 2; j++) {
			data.push_back(temp2[j]);
		}

		// fork status (if occupied: 100 else 0). This can later be extended to hold a percentage value
		data.push_back(signedChar(*forkOccupied1));	// first set of forks
		data.push_back(signedChar(*forkOccupied2));	// second set of forks
	}
}

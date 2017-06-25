/*
 * LocalizationToDatagramTranslator.cpp
 *
 * Export localization information via a datagram
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "LocalizationToDatagramTranslator.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::LocalizationToDatagramTranslator::LocalizationToDatagramTranslator(int vehicleID) {
	vehicleID_ = vehicleID;
	packetType_ = 0;
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::LocalizationToDatagramTranslator::~LocalizationToDatagramTranslator() {
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
void dsa::LocalizationToDatagramTranslator::translate(LocalizationResult *localizationResult, ForkHeightResult *forkHeightResult, std::vector<char> &result) {
	if(localizationResult->isValid || forkHeightResult->isValid) {
		int timestamp = localizationResult->isValid ? localizationResult->timestamp : forkHeightResult->timestamp;
		// packet type 1 Byte
		result.push_back(signedChar(packetType_));
		// vehicle id 1 Byte (allows for up to 256 vehicles)
		result.push_back(signedChar(vehicleID_));
		// timestamp (milliseconds since beginning of this day)
		boost::array<char, 4> temp = splitWordToBytes(timestamp);
		for(size_t i = 0; i < 4; i++)
			result.push_back(temp[i]);
		// translation(t1, t2, t3)  4 Bytes each (in mm)
		for(size_t i = 0; i < 3; i++) {
			temp = splitWordToBytes((int)(10*localizationResult->t[i]));
			for(size_t j = 0; j < 4; j++)
				result.push_back(temp[j]);
		}
		// rotation(r1, r2, r3) 2 Bytes each (in 1/100 degrees)
		for(size_t i = 0; i < 3; i++) {
			boost::array<char, 2> temp2 = splitHalfwordToBytes((short)(100.0f*180.0f/PI*localizationResult->r[i]));
			for(size_t j = 0; j < 2; j++) {
				result.push_back(temp2[j]);
			}
		}

		// fork height 2 Bytes (in mm => max height is currently ~32,8m, which should be enough for now)
		boost::array<char, 2> temp2 = splitHalfwordToBytes(signedShort(std::max(10*forkHeightResult->h, 0.f)));
		for(size_t j = 0; j < 2; j++) {
			result.push_back(temp2[j]);
		}

		// fork tilt angle 2 Bytes (in 1/100 degrees)
		temp2 = splitHalfwordToBytes((short)(100.0f*180.0f/PI*forkHeightResult->r));
		for(size_t j = 0; j < 2; j++) {
			result.push_back(temp2[j]);
		}

		// fork status (if occupied: 100 else 0). This can later be extended to hold a percentage value
		result.push_back(signedChar(0));	// first set of forks
		result.push_back(signedChar(0));	// second set of forks
	}
}

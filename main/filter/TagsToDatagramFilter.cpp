/*
 * TagsToDatagramFilter.cpp
 *
 * Export tag information via a datagram
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "TagsToDatagramFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::TagsToDatagramFilter::TagsToDatagramFilter(int vehicleID) {
	vehicleID_ = vehicleID;
	packetType_ = 1;
	timestamp_ = 0;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::TagsToDatagramFilter::~TagsToDatagramFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::TagsToDatagramFilter::init() {
	types_ = std::vector<int>();
	contents_ = std::vector<std::string>();
	data = std::vector<char>();

	types = &types_;
	contents = &contents_;
	timestamp = &timestamp_;
}

void dsa::TagsToDatagramFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	data.clear();
}

/**
 * The payload of the packet always contains the following information in the given order:
 *
 * 1. packet type as char: 1 Byte (currently we only have one packet type which is "0" and contains the fork status
 * 2. vehicle id as char: 1 Byte (allows for up to 256 vehicles)
 * 3. timestamp as int: 4 Bytes (milliseconds since beginning of this day)
 * for each tag follows:
 * A. type as char: 1 Byte
 * B. length as char: 1 Byte
 * C. contents as char[] with length given above
 * next tag... etc.
 */
void dsa::TagsToDatagramFilter::evaluate() {
	if(types->size() > 0) {
		// packet type 1 Byte
		data.push_back(signedChar(packetType_));
		// vehicle id 1 Byte (allows for up to 256 vehicles)
		data.push_back(signedChar(vehicleID_));
		// timestamp (milliseconds since beginning of this day)
		boost::array<char, 4> temp = splitWordToBytes((int)(*timestamp));
		for(size_t i = 0; i < 4; i++)
			data.push_back(temp[i]);
		// tag info
		for(size_t i = 0; i < types->size(); i++) {
			data.push_back(signedChar(types->at(i)));	// type
			std::string content = contents->at(i);
			data.push_back(signedChar(content.size()));	// length
			for(size_t j = 0; j < content.size(); j++)
				data.push_back(content.at(j));	// contents
		}
	}
}

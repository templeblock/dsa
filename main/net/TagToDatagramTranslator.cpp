/*
 * TagToDatagramTranslator.cpp
 *
 * Export barcode information via a datagram
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "TagToDatagramTranslator.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::TagToDatagramTranslator::TagToDatagramTranslator(int vehicleID) {
	vehicleID_ = vehicleID;
	packetType_ = 1;
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::TagToDatagramTranslator::~TagToDatagramTranslator() {
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
void dsa::TagToDatagramTranslator::translate(BarcodeResult *barcodeResult, std::vector<char> &result) {
	if(barcodeResult->isValid) {
		// packet type 1 Byte
		result.push_back(signedChar(packetType_));
		// vehicle id 1 Byte (allows for up to 256 vehicles)
		result.push_back(signedChar(vehicleID_));
		// timestamp (milliseconds since beginning of this day)
		boost::array<char, 4> temp = splitWordToBytes(barcodeResult->timestamp);
		for(size_t i = 0; i < 4; i++)
			result.push_back(temp[i]);
		// tag info
		for(size_t i = 0; i < barcodeResult->barcodeEvents.size(); i++) {
			result.push_back(signedChar(barcodeResult->barcodeEvents.at(i).barcode.format));	// type
			std::string content = barcodeResult->barcodeEvents.at(i).barcode.contents;
			result.push_back(signedChar(content.size()));	// length
			for(size_t j = 0; j < content.size(); j++)
				result.push_back(content.at(j));	// contents
		}
	}
}

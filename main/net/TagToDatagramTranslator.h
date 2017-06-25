/*
 * TagToDatagramTranslator.cpp
 *
 * Export tag information via a datagram
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef TAGTODATAGRAMTRANSLATOR_H_
#define TAGTODATAGRAMTRANSLATOR_H_

#include <vector>
#include <boost/array.hpp>
#include <generic/helper.h>
#include <demonstrator/BarcodeResult.h>

namespace dsa {

class TagToDatagramTranslator {
	int packetType_;
	int vehicleID_;

public:

	TagToDatagramTranslator(int vehicleID);
	virtual ~TagToDatagramTranslator();

	void translate(BarcodeResult *barcodeResult, std::vector<char> &result);
};

}
#endif /* TAGTODATAGRAMTRANSLATOR_H_ */

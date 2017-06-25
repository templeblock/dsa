/*
 * LocalizationToDatagramTranslator.cpp
 *
 * Export localization information via a datagram
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef LOCALIZATIONTODATAGRAMTRANSLATOR_H_
#define LOCALIZATIONTODATAGRAMTRANSLATOR_H_

#include <vector>
#include <boost/array.hpp>
#include <generic/helper.h>
#include <demonstrator/LocalizationResult.h>
#include <demonstrator/ForkHeightResult.h>

namespace dsa {

class LocalizationToDatagramTranslator {
	int packetType_;
	int vehicleID_;

public:

	LocalizationToDatagramTranslator(int vehicleID);
	virtual ~LocalizationToDatagramTranslator();

	void translate(LocalizationResult *localizationResult, ForkHeightResult *forkHeightResult, std::vector<char> &result);
};

}
#endif /* LOCALIZATIONTODATAGRAMTRANSLATOR_H_ */

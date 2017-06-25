/*
 * Grabilter.cpp
 *
 *	Grabs an image from a Videohub
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "GrabFilter.h"

/**
 * Empty constructor.
 */
dsa::GrabFilter::GrabFilter(DataHub &dataHub) {
	input_ = &dataHub;
	//apply();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::GrabFilter::~GrabFilter() {
	if(data != NULL)
		postprocessing();
}

void dsa::GrabFilter::apply() {
	data = input_->getData(false);
	dataTimestamp = data->getDataTimestamp();
}

void dsa::GrabFilter::preprocessing() {
}

void dsa::GrabFilter::postprocessing() {
	delete data;
	data = NULL;
}


void dsa::GrabFilter::setInput(DataHub* const dataHub) {
	input_ = dataHub;
}

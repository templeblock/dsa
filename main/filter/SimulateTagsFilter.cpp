/*
 * SimulateTagsFilter.h
 *
 *	This filter simulates tag information for testing purposes.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "SimulateTagsFilter.h"

/**
 * Constructor
 */
dsa::SimulateTagsFilter::SimulateTagsFilter(int maxTags, int maxTagLength) {
	maxTags_ = maxTags;
	maxTagLength_ = maxTagLength;

	timestamp = 0;
	types = std::vector<int>();
	contents = std::vector<std::string>();

	init();
}

/**
 * Destructor
 */
dsa::SimulateTagsFilter::~SimulateTagsFilter() {
}

bool dsa::SimulateTagsFilter::isReady() const {
	return(true);
}

void dsa::SimulateTagsFilter::init() {
}

void dsa::SimulateTagsFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	types.clear();
	contents.clear();
}

/**
 * Fill internal data structure with markers
 */
void dsa::SimulateTagsFilter::evaluate() {
	size_t ntags = randUnsignedShort()%maxTags_+1;
	for(size_t i = 0; i < ntags; i++) {
		// type
		int type = randUnsignedShort()%18;
		types.push_back(type);
		// content
		size_t length = randUnsignedShort()%maxTagLength_+1;
		std::string content = std::string("");
		for(size_t j = 0; j < length; j++)
			content.append(intToString(randUnsignedShort()%10)) ;
		contents.push_back(content);
	}
	timestamp = currentTimeOfDayMilliseconds();
	std::cout << "timestamp=" << timestamp << " type=" << types[0] << " contents=" << contents[0] << std::endl;
}

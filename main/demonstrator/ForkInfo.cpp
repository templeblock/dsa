/*
 * ForkInfo.cpp
 *
 *  Created on: 17.04.2013
 *      Author: jung
 */

#include "ForkInfo.h"

dsa::ForkInfo::ForkInfo(ForkState *left, ForkState *right) {
		left_ = left;
		right_ = right;
		lastKnown_ = unknown;	// by default last state is unknown
		details = " (" + ForkInfoText[*left_] + " " + ForkInfoText[*right_] + ")";
		text = ForkInfoText[getState()];
		std::transform(text.begin(), text.end(), text.begin(), ::toupper);
		text.append(details);
}

dsa::ForkInfo::~ForkInfo() {
}

dsa::ForkState dsa::ForkInfo::getState() {
	ForkState state = getInternalState();
	if(state == idle || state == occupied)
		lastKnown_ = state;
	details = " (" + intToString(*left_) + ", " + intToString(*right_) + ")";
	if(state == lost) {
		text = ForkInfoText[lost];
		state = unknown;
	} else {
		text = ForkInfoText[state];
	}
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);
	text.append(details);
	return(state);
}

dsa::ForkState dsa::ForkInfo::getInternalState() {
	if(*left_ == *right_) {
		return(*left_);
	} else if(*left_ == lost || *right_ == lost) {	// one lost => lost
		return(lost);
	} else if(*left_ == unknown) { // left unknown => right
		return(*right_);
	} else if(*right_ == unknown) {  // right unknown => left
		return(*left_);
	} else if((*left_ + *right_) == (idle + occupied)) {	// opposites (one idle one occupied) => last known state
		return(lastKnown_);
	} else {
		return(unknown); // unknown
	}
}

dsa::ForkState dsa::ForkInfo::getLeft() {
	ForkState state = *left_;
	return(state);
}

dsa::ForkState dsa::ForkInfo::getRight() {
	ForkState state = *right_;
	return(state);
}

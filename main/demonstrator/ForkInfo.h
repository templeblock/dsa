/*
 * ForkInfo.h
 *
 *  Created on: 17.04.2013
 *      Author: jung
 */

#ifndef FORKINFO_H_
#define FORKINFO_H_

#include <generic/helper.h>

static const std::string ForkInfoText[4] = {"idle", "occupied", "unknown", "lost"};

namespace dsa {

class ForkInfo {

	ForkState *left_;
	ForkState *right_;
	ForkState lastKnown_;

	ForkState getInternalState();

public:

	std::string text;
	std::string details;

	ForkInfo(ForkState *left, ForkState *right);
	virtual ~ForkInfo();

	ForkState getState();
	ForkState getLeft();
	ForkState getRight();

};

}
#endif /* FORKINFO_H_ */

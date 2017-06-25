/*
 * ForkPerimeterFinderFilter.h
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef FORKPERIMETERFINDERFILTER_H_
#define FORKPERIMETERFINDERFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class ForkPerimeterFinderFilter : public ImageFilter {
	void init();
	void apply();

public:
	ForkPerimeterFinderFilter();
	virtual ~ForkPerimeterFinderFilter();
};

}
#endif /* FORKPERIMETERFINDERFILTER_H_ */

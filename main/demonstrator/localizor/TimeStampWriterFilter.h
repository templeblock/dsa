/*
 * TimeStampWriterFilter.h
 *
 *  Created on: 10.04.2013
 *      Author: Bigontina
 */

#ifndef TIMESTAMPWRITERFILTER_H_
#define TIMESTAMPWRITERFILTER_H_

#include <generic/types.h>
#include "filter/ImageFilter.h"

namespace dsa {

class TimeStampWriterFilter : public ImageFilter {

	void init();

public:
	TimeStampWriterFilter();

	void apply();
};

}
#endif /* TIMESTAMPWRITERFILTER_H_ */

/*
 * ImageBufferFilter.h
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef IMAGEBUFFERFILTER_H_
#define IMAGEBUFFERFILTER_H_

#include <sensordata/ImageBuffer.h>
#include "EvaluationFilter.h"

namespace dsa {

class ImageBufferFilter : public EvaluationFilter {

protected:

	ImageBuffer buffer_;
	void evaluate();

public:

	ImageBufferFilter(const size_t size=10);
	virtual ~ImageBufferFilter();
};

}
#endif /* IMAGEBUFFERFILTER_H_ */

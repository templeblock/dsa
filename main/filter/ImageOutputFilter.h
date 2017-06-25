/*
 * ImageOutputFilter.h
 *
 *	Shows the input image without modifications in a window.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef IMAGEOUTPUTFILTER_H_
#define IMAGEOUTPUTFILTER_H_

#include "EvaluationFilter.h"

namespace dsa {

class ImageOutputFilter : public EvaluationFilter {

	std::string windowName_;

	void init();
	void evaluate();

public:

	ImageOutputFilter(const std::string &windowName);
	virtual ~ImageOutputFilter();

	void setActive(const bool active);
};

}
#endif /* IMAGEOUTPUTFILTER_H_ */

/*
 * EvaluationFilter.h
 *
 *	Is essentially an ImageFilter, except that the input image itself is not changed in any way, but passed on
 *	for further consideration.
 *	An additional function named evaluate(), which subclasses have to implement is used to extract
 *	information from the image.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef EVALUATIONFILTER_H_
#define EVALUATIONFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class EvaluationFilter: public ImageFilter {

protected:

	virtual void evaluate() = 0;
	virtual void init();

public:

	EvaluationFilter();
	virtual ~EvaluationFilter();

	virtual void apply();
};

}
#endif /* EVALUATIONFILTER_H_ */

/*
 * EqualizeHistFilter.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/histograms.html#equalizehist
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef EQUALIZEHISTFILTER_H_
#define EQUALIZEHISTFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class EqualizeHistFilter : public ImageFilter {

	void init();

public:

	EqualizeHistFilter();
	virtual ~EqualizeHistFilter();

	void apply();
};

}
#endif /* EQUALIZEHISTFILTER_H_ */

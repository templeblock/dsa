/*
 * ImageBufferMeanFilter.h
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef IMAGEBUFFERMEANFILTER_H_
#define IMAGEBUFFERMEANFILTER_H_

#include "ImageBufferFilter.h"

namespace dsa {

class ImageBufferMeanFilter : public ImageBufferFilter {

protected:

	bool initialized_;
	cv::Mat sum_;

	void apply();
	void evaluate();

public:

	ImageBufferMeanFilter(const size_t size=10);
	virtual ~ImageBufferMeanFilter();
};

}
#endif /* IMAGEBUFFERMEANFILTER_H_ */

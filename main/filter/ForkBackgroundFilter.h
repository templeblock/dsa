/*
 * ForkBackgroundFilter.h
 *
 * 	Filter background out from our view on the forks
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef FORKBACKGROUNDFILTER_H_
#define FORKBACKGROUNDFILTER_H_

#include <generic/helper.h>
#include "ImageBufferFilter.h"

namespace dsa {

class ForkBackgroundFilter : public ImageBufferFilter {

	bool isColor_;
	UINT32 prev_nruns_;
	UINT32 nruns_;
	cv::Mat image_;
	cv::Mat average_;

	void evaluate();

public:

	ForkBackgroundFilter(bool isColor=false);
	virtual ~ForkBackgroundFilter();

	void reset();
	bool usedLastImage();
	cv::Mat getAverage();
};

}
#endif /* FORKBACKGROUNDFILTER_H_ */

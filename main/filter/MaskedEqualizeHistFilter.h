/*
 * MaskedEqualizeHistFilter.h
 *
 * 	Ordinary histogram equalization performed on parts of an image defined by a mask
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/histograms.html#equalizehist
 *
 *	for details on the histogram equalization.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef MASKEDEQUALIZEHISTFILTER_H_
#define MASKEDEQUALIZEHISTFILTER_H_

#include <generic/types.h>
#include <opencv2/core/internal.hpp>
#include "ImageFilter.h"

namespace dsa {

class MaskedEqualizeHistFilter : public ImageFilter {

	cv::Mat mask_;

	void init();

public:

	cv::Mat* mask;

	MaskedEqualizeHistFilter(cv::Mat &mask);
	virtual ~MaskedEqualizeHistFilter();

	void apply();
};

}
#endif /* MASKEDEQUALIZEHISTFILTER_H_ */

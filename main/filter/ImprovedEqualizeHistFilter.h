/*
 * ImprovedEqualizeHistFilter.h
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

#ifndef IMPROVEDEQUALIZEHISTFILTER_H_
#define IMPROVEDEQUALIZEHISTFILTER_H_

#include <generic/types.h>
#include <array>
#include "ImageFilter.h"
#include <opencv2/core/internal.hpp>

namespace dsa {

class ImprovedEqualizeHistFilter : public ImageFilter {

	class BorderCalculator {

		std::vector<bool> bordersCalculated_;
		std::vector<float> lowerBorder_;
		std::vector<float> intervalWidth_;
		std::array<float, 2> (BorderCalculator::*getIntervalArray[2])(UINT8 i1, UINT8 i2, UINT8 i3, UINT8 pos);

		std::array<float, 2> calcInterval(UINT8 i1, UINT8 i2, UINT8 i3, UINT8 pos);
		std::array<float, 2> getIntervalFromBuffer(UINT8 i1, UINT8 i2, UINT8 i3, UINT8 pos);

	public:
		BorderCalculator(size_t n);
		virtual ~BorderCalculator();

		std::array<float, 2> getInterval(UINT8 i1, UINT8 i2, UINT8 i3, UINT8 pos);
	};

	cv::Mat kernel_;

	void init();

public:

	ImprovedEqualizeHistFilter(int ksize=3, float centerWeight=1.0f/9.0f);
	virtual ~ImprovedEqualizeHistFilter();

	void apply();
};

}
#endif /* IMPROVEDEQUALIZEHISTFILTER_H_ */

/*
 * BarcodeSegmentationFilter.cpp
 *
 * 	See
 *
 *	barcode_extraction_framework.pdf
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef BARCODESEGMENTATIONFILTER_H_
#define BARCODESEGMENTATIONFILTER_H_

#include "EvaluationFilter.h"
#include "helper/BarcodeSegmenter.h"

namespace dsa {

class BarcodeSegmentationFilter : public EvaluationFilter {

	BarcodeSegmenter BarcodeSegmenter_;

	void init();

public:

	std::vector<Roi> candidates;

	BarcodeSegmentationFilter(const int ksize);
	virtual ~BarcodeSegmentationFilter();

	void evaluate();

	void preprocessing();
};

}
#endif /* BARCODESEGMENTATIONFILTER_H_ */

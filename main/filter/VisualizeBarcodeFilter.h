/*
 * VisualizeBarcodeFilter.h
 *
 *	Mark barcodes in an image and display information on them inline
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef VISUALIZEBARCODEFILTER_H_
#define VISUALIZEBARCODEFILTER_H_

#include <generic/types.h>
#include "ImageFilter.h"

namespace dsa {

class VisualizeBarcodeFilter : public ImageFilter {

	size_t maxContentsSize_;
	int fontFace_;
	double fontScale_;
	cv::Scalar color_;
	int thickness_;
	int lineType_;

	void init();

public:

	std::vector<Barcode>* codes;

	VisualizeBarcodeFilter(size_t maxContentsSize=0, int fontFace=cv::FONT_HERSHEY_COMPLEX, double fontScale=1.0, cv::Scalar color=cv::Scalar(0,255,0), int thickness=2, int lineType=8);
	virtual ~VisualizeBarcodeFilter();

	void apply();
};

}
#endif /* VISUALIZEBARCODEFILTER_H_ */

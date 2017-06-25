/*
 * ZXingBarcodeAnalysisFilter.h
 *
 *	This filter determines the contents of a extracted barcode image
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef ZXINGBARCODEANALYSISFILTER_H_
#define ZXINGBARCODEANALYSISFILTER_H_

#include <generic/types.h>
#include <sensordata/Mat2LuminanceSource.h>
#include "EvaluationFilter.h"
#include "zxing/BarcodeFormat.h"
#include "zxing/common/GlobalHistogramBinarizer.h"
#include "zxing/common/HybridBinarizer.h"
#include "zxing/MultiFormatReader.h"

namespace dsa {

class ZxingBarcodeAnalysisFilter : public EvaluationFilter {

	zxing::DecodeHints hints_;
	zxing::MultiFormatReader reader_;

	Barcode decode(cv::Mat &image);
	void init();
	void evaluate();

public:

	std::vector<Roi>* candidates;
	std::vector<Barcode> codes;

	ZxingBarcodeAnalysisFilter(zxing::DecodeHints &hints);
	virtual ~ZxingBarcodeAnalysisFilter();

	void preprocessing();
};

}
#endif /* ZXINGBARCODEANALYSISFILTER_H_ */

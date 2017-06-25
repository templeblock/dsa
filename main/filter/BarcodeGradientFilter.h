/*
 * BarcodeGradientFilter.h
 *
 * 	See
 *
 *	barcode_poster_taller_0.pdf
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef BARCODEGRADIENTFILTER_H_
#define BARCODEGRADIENTFILTER_H_

#include <vector>
#include <generic/types.h>
#include "ImageFilter.h"

namespace dsa {

class BarcodeGradientFilter : public ImageFilter {

	void init();

public:

	BarcodeGradientFilter();
	virtual ~BarcodeGradientFilter();

	void apply();
};

}
#endif /* BARCODEGRADIENTFILTER_H_ */

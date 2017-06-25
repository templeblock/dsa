/*
 * BarcodeScanlineFilter.h
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

#ifndef BARCODESCANLINEFILTER_H_
#define BARCODESCANLINEFILTER_H_

#include <vector>
#include "ImageFilter.h"

namespace dsa {

class BarcodeScanlineFilter : public ImageFilter {

	void init();
	inline int direction(int x1, int x2) {
		if(x2 > x1)
			return(2);
		else
			return((x2 < x1) ? 1 : 0);
	}

	int countChanges(std::vector<int> &v);

public:

	BarcodeScanlineFilter();
	virtual ~BarcodeScanlineFilter();

	void apply();
};

}
#endif /* BARCODESCANLINEFILTER_H_ */

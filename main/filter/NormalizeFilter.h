/*
 * NormalizeFilter.h
 *
 *	Experimental normalization filter different from equalizehist
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef NORMALIZEFILTER_H_
#define NORMALIZEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class NormalizeFilter : public ImageFilter {
	double alpha_;
	double beta_;
	int normType_;
	int rtype_;
	cv::Mat mask_;

	void init();

public:
	double* alpha;
	double* beta;
	int* normType;
	int* rtype;
	cv::Mat* mask;

	NormalizeFilter(double alpha=1, double beta=0, int normType=cv::NORM_L2, int rtype=-1, const cv::Mat& mask=cv::Mat());
	virtual ~NormalizeFilter();

	void apply();
};

}
#endif /* NORMALIZEFILTER_H_ */

/*
 * BackgroundImageFilter.h
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#ifndef BACKGROUNDIMAGEFILTER_H_
#define BACKGROUNDIMAGEFILTER_H_

#include "ImageFilter.h"

namespace dsa {

class BackgroundImageFilter : public ImageFilter {

	bool supplyForeground_;
    cv::Mat background_;
    cv::Mat foreground_;
    cv::BackgroundSubtractorMOG2 bgs_;

	void init();

public:

	bool* supplyForeground;

	BackgroundImageFilter(int history,  float varThreshold, bool bShadowDetection=true, bool supplyForeground=true);
	virtual ~BackgroundImageFilter();

	void apply();
};

}
#endif /* BACKGROUNDIMAGEFILTER_H_ */

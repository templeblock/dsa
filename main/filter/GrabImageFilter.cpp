/*
 * GrabImageFilter.cpp
 *
 *	Grabs an image from a Videohub
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "GrabImageFilter.h"

/**
 * Empty constructor.
 */
dsa::GrabImageFilter::GrabImageFilter(VideoHub &videoHub) : dsa::GrabFilter(videoHub) {
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::GrabImageFilter::~GrabImageFilter() {
}

void dsa::GrabImageFilter::init() {
}

void dsa::GrabImageFilter::bypass() {
}

bool dsa::GrabImageFilter::isReady() const {
	return(!((ImageSensorData*)data)->getImage().empty());
}

void dsa::GrabImageFilter::apply() {
	GrabFilter::apply();
	if(isReady()) {
		dst = ((ImageSensorData*)data)->getImage();
      //std::cout<<std::endl<<"saving image "<<std::endl;
      //cv::imwrite("cv_imageSave.jpg",dst);
	}
	
	

}

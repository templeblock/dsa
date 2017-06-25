/*
 * MarkerGeneration.hpp
 *
 *  Created on: 06.03.2013
 *      Author: Bigontina
 */

#ifndef MARKERGENERATION_HPP_
#define MARKERGENERATION_HPP_

#include <aruco/arucofidmarkers.h>
#include <opencv2/opencv.hpp>

using namespace cv;


/*
 * Creates the marker image and adds an additional white border.
 * The argument 'size' specifies the size of the marker, however, due to the border
 *   the returned image will have the size: size*9/7
 */
Mat createMarker(int id, int size=7, bool addborder=true) {
	Mat markerimgo = aruco::FiducidalMarkers::createMarkerImage(id, size);
	// add white Border to Image:
	int bordersize = round(markerimgo.rows/7);
	Mat markerimg = Mat::ones(markerimgo.rows+2*bordersize, markerimgo.cols+2*bordersize, markerimgo.type());
	markerimg = markerimg * 255;
	markerimgo.copyTo(
			markerimg.rowRange(bordersize,markerimgo.rows+bordersize)
				.colRange(bordersize, markerimgo.cols+bordersize));

	return markerimg;
}


#endif /* MARKERGENERATION_HPP_ */

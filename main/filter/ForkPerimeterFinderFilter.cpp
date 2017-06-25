/*
 * ForkPerimeterFinderFilter.cpp
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "ForkPerimeterFinderFilter.h"
#include <iostream>

dsa::ForkPerimeterFinderFilter::ForkPerimeterFinderFilter() {
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::ForkPerimeterFinderFilter::~ForkPerimeterFinderFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::ForkPerimeterFinderFilter::init() {
	//EvaluationFilter::init();
}

/**
 * Determine Fork
 *
 */
void dsa::ForkPerimeterFinderFilter::apply() {
	const double maxWidthRatio = 1.0/3.0;	// max value for width/length
	const double forkFOVratioX = 1.0/3.0;	// see  above
	const double minForkPerimeter = 2 * forkFOVratioX * src->cols;
	const double minForkLengthRatio = 0.5;
	const double maxForkAngleDifference = 40.0;	// maximal difference in angle between the forks
	const double forkBorderMarginRatio = 0.05;	// how far away from the border of the image may the forks start (ratio of the length of the border involved)

	const int maxYTop = (int)(src->rows * forkBorderMarginRatio); // minimum Y value to be near the bottom border
	const int maxXLeft = (int)(src->cols * forkBorderMarginRatio); // maximum X value to be near the left border
	const int minYBottom = (int)(src->rows * (1 - forkBorderMarginRatio)); // minimum Y value to be near the bottom border
	const int minXRight = (int)(src->cols * (1 - forkBorderMarginRatio)); // minimum X value to be near the right border

	std::vector<std::vector<cv::Point> > contours;
	std::vector<std::vector<cv::Point> > polygons;
	std::vector<std::vector<char> > adjacentLines;
	std::vector<double> lengths;
	std::vector<double> angles;
	dst = cv::Mat::zeros(src->rows, src->cols, CV_8UC1);

	cv::findContours(*src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for(size_t i = 0; i < contours.size(); i++) {
		cv::Mat polygon(contours[i]);
		double perimeter = arcLength(polygon, true);
		if(perimeter >= minForkPerimeter) {	// filter small polygones to speed up process (saves computation time). The real length check is performed below.
			std::vector<cv::Point> approxpolygon, hull;
			// approximate contour by polygon
			cv::approxPolyDP(polygon, approxpolygon, perimeter*0.01, true);
			// modify polygon to its convex hull
			cv::Mat convexPolygon(approxpolygon);
			cv::convexHull(convexPolygon, hull); // get back edges that were bitten off by lighting conditions

			double perimeter = arcLength(polygon, true); // check length again for convex hull, as the arclength of the convex hull is shorter but describes our fork more precisely
			if(perimeter >= minForkPerimeter) {	// filter small polygones by perimeter
				// approximate length and width of the polygon by length and width of the bounding rotated rectangle with the minimal area
				double area = contourArea(hull);
				cv::Rect forkBoundingRect = boundingRect(hull);
				double length = sqrt(forkBoundingRect.width*forkBoundingRect.width + forkBoundingRect.height*forkBoundingRect.height);
				double width = area/length;
				if(width/length < maxWidthRatio) {
					// check if two consecutive points in the polygon are near the image border
					std::set<char> borders;
					for(size_t i = 0; i < hull.size(); i++) {
						cv::Point current = hull[i];
						cv::Point next = hull[(i+1)%hull.size()];
						// one line cannot be near top/bottom and left/right at the same time, but all other combinations are possible
						if(current.y <= maxYTop && next.y <= maxYTop)	// near top
							borders.insert(0);
						else if(current.y >= minYBottom && next.y >= minYBottom)	// near bottom
							borders.insert(2);
						if((current.x <= maxXLeft && next.x <= maxXLeft)) // near left
							borders.insert(1);
						else if((current.x >= minXRight && next.x >= minXRight)) // near right
							borders.insert(3);
					}
					if(borders.size() == 1 || borders.size() == 2) { // we are close to 1 or 2 borders
						cv::fillConvexPoly(dst, hull, cv::Scalar(255));
					} // else not a fork
				}
			}
		}
	}
}

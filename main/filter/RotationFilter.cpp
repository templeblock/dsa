/*
 * RotationFilter.cpp
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#warpaffine
 *
 *	for details on the image filter.
 *
 *  Created on: 03.07.2012
 *      Author: jung
 */

#include "RotationFilter.h"

/**
 *
 * @param angle Rotation angle in degrees
 * @param dsize desired Size of the image (determines scale factor)
 */
dsa::RotationFilter::RotationFilter(double angle, cv::Size dsize) {
	angle_ = angle;
	dsize_ = dsize;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::RotationFilter::~RotationFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::RotationFilter::init() {
	angle = &angle_;
	dsize = &dsize_;
}

bool dsa::RotationFilter::isReady() {
	return(dsize->width != src->cols || dsize->height != src->rows || *angle != 0);
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::RotationFilter::apply() {
	if(isReady()) {
		cv::Point2f center(src->cols/2.0f, src->rows/2.0f);	// always rotate image around center (thats what this filter is for)

		// calculate scale factor from given target size
		double scaleX = dsize->width/(double)src->cols;
		double scaleY = dsize->height/(double)src->rows;

		// map interval [0; 360] -> [-180; 180]
		double degAngle = *angle;
		if(degAngle > 180.0)
			degAngle -= 360.0;
		else if(degAngle < -180.0)
			degAngle += 360.0;

		// sine and cosine take radians as argument, hence transform degree to radians (*PI/180)
		double radAngle = degAngle * CV_PI/180.0;
		double sinAngle = sin(radAngle);
		double cosAngle = cos(radAngle);

		// compute rotation matrix. we do not use the getRotationMatrix2D method, as it seems to suffer from a bug when also scaling at the same time (image is not completely within target size)
		cv::Mat m = cv::Mat(2, 3, CV_64F);
		// transformation part
		m.at<double>(0, 0) = scaleX*cosAngle;
		m.at<double>(0, 1) = scaleY*sinAngle;
		m.at<double>(1, 0) = -scaleX*sinAngle;
		m.at<double>(1, 1) = scaleY*cosAngle;
		// offset and target window size
		double xoffset;
		double yoffset;
		cv::Size targetSize;
		if((degAngle < 45.0 && degAngle > -45.0) || degAngle > 135.0 || degAngle < -135.0) {
			targetSize = cv::Size(scaleX*src->cols, scaleY*src->rows);
			xoffset = scaleX * center.x;
			yoffset = scaleY * center.y;
		} else {
			targetSize = cv::Size(scaleY*src->rows, scaleX*src->cols);
			xoffset = scaleY * center.y;
			yoffset = scaleX * center.x;
		}
		m.at<double>(0, 2) = -scaleX*cosAngle*center.x - scaleY*sinAngle*center.y + xoffset;
		m.at<double>(1, 2) = scaleX*sinAngle*center.x - scaleY*cosAngle*center.y + yoffset;

		// apply matrix to affine transform
		cv::warpAffine(*src, dst, m, targetSize);

		// test watershed segmentation
		/*cv::Mat colorImg;
		cv::cvtColor(dst, colorImg, CV_GRAY2RGB);

		// mark background
		const cv::Scalar color_bg = cv::Scalar(1);
		const cv::Scalar color_fork = cv::Scalar(std::numeric_limits<int>::max()); //
		int line_y = 0.1*dst.rows;

		const double maxWidthRatio = 1.0/5.0;	// max value for width/length
		const double minWidthRatio = 0.05; // min value for width/length
		const double forkFOVratioY = 1.0/3.0;	// see  above
		const double minForkPerimeter = 2 * forkFOVratioY * std::max(dst.cols, dst.rows);	// 1/3 der längeren Seite
		const double minForkLengthRatio = 0.5;
		const double minForkWidthRatio = 0.5;
		const double maxForkAngleDifference = 40.0;	// maximal difference in angle between the forks
		const double forkBorderMarginRatio = 0.05;	// how far away from the border of the image may the forks start (ratio of the length of the border involved)
		const int maxYTop = (int)(dst.rows * forkBorderMarginRatio); // minimum Y value to be near the bottom border
		const int maxXLeft = (int)(dst.cols * forkBorderMarginRatio); // maximum X value to be near the left border
		const int minYBottom = (int)(dst.rows * (1 - forkBorderMarginRatio)); // minimum Y value to be near the bottom border
		const int minXRight = (int)(dst.cols * (1 - forkBorderMarginRatio)); // minimum X value to be near the right border

		size_t s_points = 10;	// we actually sample 1 more to reduce code complexity
		std::vector<std::vector<cv::Point> > polygons;
		cv::Mat binaryImg;
		for(size_t i = 0; i <= s_points; i++) {
			if(i != s_points/2) {
				cv::Mat markers = cv::Mat::zeros(dst.rows, dst.cols, CV_32SC1);
				line(markers, cv::Point(0, line_y), cv::Point(markers.cols-1,line_y), color_bg, 5);
				cv::circle(markers, cv::Point((markers.cols)/2, 2*(markers.rows)/3), 5, color_bg, -1);
				cv::circle(markers, cv::Point(i*(markers.cols)/s_points, 2*(markers.rows)/3), 5, color_fork, -1);
				//imshow("Markers", markers);
				cv::watershed(colorImg, markers); // input image
				//imshow("Watershed", markers);
				markers.convertTo(binaryImg, CV_8U);
				threshold(binaryImg, binaryImg, 128, 255, cv::THRESH_BINARY);
				//cv::imshow("Segmentation result", binaryImg);

				// find contours and add them for each sheds
				std::vector<std::vector<cv::Point> > contours_temp;
				cv::findContours(binaryImg, contours_temp, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

				for(std::vector<std::vector<cv::Point> >::iterator it = contours_temp.begin(); it != contours_temp.end(); it++) {
					cv::Mat polygon(*it);
					double perimeter = arcLength(polygon, true);
					std::vector<cv::Point> approxpolygon, hull;
					// approximate contour by polygon
					cv::approxPolyDP(polygon, approxpolygon, perimeter*0.01, true);
					// modify polygon to its convex hull
					cv::Mat convexPolygon(approxpolygon);
					cv::convexHull(convexPolygon, hull); // get back edges that were bitten off by lighting conditions

					polygons.push_back(hull);
				}
			}
		}

		// work on all sheds at once
		for(std::vector<std::vector<cv::Point> >::iterator it = polygons.begin(); it != polygons.end(); it++) {
			// approximate length and width of the polygon by length and width of the bounding rotated rectangle with the minimal area
			double area = contourArea(*it);
			cv::Rect forkBoundingRect = boundingRect(*it);
			//double length = sqrt(forkBoundingRect.width*forkBoundingRect.width + forkBoundingRect.height*forkBoundingRect.height);
			double length = polygonLength(*it);
			double width = area/length;
			double widthByLength = width/length;

			std::cerr << length << ", " << width << ", " << widthByLength << std::endl;
			if(width < binaryImg.cols/6 && widthByLength < maxWidthRatio && widthByLength > minWidthRatio) {
				// check if two consecutive points in the polygon are near the image border
				std::set<char> borders;
				for(size_t j = 0; j < it->size(); j++) {
					cv::Point current = (*it)[j];
					cv::Point next = (*it)[(j+1)%it->size()];
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
					//cv::Mat output = cv::Mat::zeros(binaryImg.rows, binaryImg.cols, CV_8UC1); // mask is initially 0 everywhere
					//cv::fillConvexPoly(output, hull, cv::Scalar(255));
					cv::Mat matHull(*it);
					std::vector<cv::Mat> polylines;
					polylines.push_back(matHull);
					cv::Mat copy;
					cv::cvtColor(dst, copy, CV_GRAY2RGB);
					cv::polylines(copy, polylines, true, cv::Scalar(0,255,0), 5);
					imshow("clone", copy);
					cv::waitKey();
				}
			}
		}*/
	} else {
		dst = *src;	// pass-though src, since we do not modify it (copies headers, references data)
	}
}

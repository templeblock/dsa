/*
 * SquareDetector.cpp
 *
 *  Created on: 27.03.2012
 *      Author: jung
 */

#include "SquareDetector.h"

dsa::SquareDetector::SquareDetector(const cv::Mat& mat, const int lengthThreshold, const float opposingThreshold, const float adjacentThreshold, const float cosineThreshold) {
	image_ = mat.clone();
	lengthThreshold_ = lengthThreshold;
	opposingThreshold_ = opposingThreshold;
	adjacentThreshold_ = adjacentThreshold;
	cosineThreshold_ = cosineThreshold;
}

dsa::SquareDetector::~SquareDetector() {
}

/**
 * Identify areas of interest (AOI) which could contain the feature we are looking for.
 *
 * We find relevant quadrilaterals with the following features:
 * 1. each edge has a length of at least lengthThreshold
 * 2. opposing edges are roughly equally long (tolerance: opposingEdgesMaxThreshold)
 * 3. opposing edges are roughly equally long (tolerance: adjacentEdgesMaxThreshold)
 * 4. the angle between opposing edges is not too big (tolerance: minOpposingEdgeCosine)
 *
 * @param mat: the image in which the AOI are identified in (b/w image)
 *
 * @return a pointer to a SquareList
 */
dsa::SquareList* dsa::SquareDetector::find() {
	SquareList* squareList = new SquareList();
    std::vector< std::vector<cv::Point> > contours;
    cv::findContours(image_, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	int a[] = { 0, 0, 0, 0, 0, 0 };
    for(UINT32 i = 0; i < contours.size(); i++) {
	    std::vector<cv::Point> contour = contours[i];
	    const UINT32 contourSize = contour.size();
	    if(contourSize >= 10) {	// something with < 4 Points will not be approximated as square anyway (experimentally: 10 are enough)
	    	a[0]++;
	    	cv::Mat polygon(contour);
	    	std::vector<cv::Point> p;
	    	cv::approxPolyDP(polygon, p, arcLength(polygon, true)*0.02, true);

	    	// first criterion: 4 edges
	    	if(p.size() == 4) {
	    		a[1]++;

	    		float vectorLength[4];
	    		vectorLength[0] = computeLength(p[1], p[0]);
	    		vectorLength[1] = computeLength(p[2], p[1]);
	    		vectorLength[2] = computeLength(p[3], p[2]);
	    		vectorLength[3] = computeLength(p[0], p[3]);

	    		bool lengthConstraint = true;
	    		// second criterion: minimum edge length
	    		for(UINT32 j = 0; j < 4; j++) {
	    			if(vectorLength[j] < lengthThreshold_) {
	    				// one of the lines too short => definitely not identifiable as square containing 2D-Code
	    				lengthConstraint = false;
	    				break;
	    			}
	    		}

	    		if(lengthConstraint) {
	    			// third criterion: opposing edges don't differ too much in length
	    			a[2]++;
	    			float pairDeviation1 = ((vectorLength[0] > vectorLength[2]) ? (vectorLength[0])/vectorLength[2] : (vectorLength[2])/vectorLength[0]) - 1.0f;
	    			float pairDeviation2 = ((vectorLength[1] > vectorLength[3]) ? (vectorLength[1])/vectorLength[3] : (vectorLength[3])/vectorLength[1]) - 1.0f;
	    			float pair1mean = (vectorLength[0] + vectorLength[2])/2.0f;
	    			float pair2mean = (vectorLength[1] + vectorLength[3])/2.0f;
	    			float pairDeviation3 = ((pair1mean > pair2mean) ? pair1mean/pair2mean : pair2mean/pair1mean) - 1.0f;
	    			if((pairDeviation1 <= opposingThreshold_ && pairDeviation2 <= opposingThreshold_ && pairDeviation3 <= adjacentThreshold_)) {
	    				// fourth criterion: angle within interval: [45°; 135°]
	    				a[3]++;
	    				if(fabs(cosine(p[0], p[1], p[2])) < SQRT2/2.0f && fabs(cosine(p[1], p[2], p[3])) < SQRT2/2.0f) {
	    					// fifth criterion: require small angle between opposing edges
	    					a[4]++;
	    					cv::Point p2tick1 = cv::Point(p[2].x + p[0].x - p[3].x, p[2].y + p[0].y - p[3].y); // move p2->p3 vector to p0 (first pair of allegedly parallel edges)
	    					cv::Point p2tick2 = cv::Point(p[2].x + p[0].x - p[1].x, p[2].y + p[0].y - p[1].y); // move p2->p1 vector to p0 (second pair of allegedly parallel edges)

	    					if(fabs(cosine(p2tick1, p[0], p[1])) >= cosineThreshold_ && fabs(cosine(p2tick2, p[0], p[3])) >= cosineThreshold_) {
	    						a[5]++;
	    						// both angles are small
	    						// copy Points for later use
	    						Square* square = new Square();
	    						for(int i = 0; i < 4; i++)
	    							square->p[i] = cv::Point(p[i].x, p[i].y);
	    						squareList->squares.push_back(square);
	    					}
	    				}
	    			}
	    		}
	    	}
	    }
    }

    //cout << a[0] <<", " << a[1] <<", " << a[2] <<", " << a[3] <<", " << a[4] <<", " << a[5] << endl;
    return(squareList);
}

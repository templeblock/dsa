/*
 * MarkerAOIFilter.h
 *
 *	Determines the area surrounding our markers.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "MarkerAOIFilter.h"

/**
 * Constructor
 */
dsa::MarkerAOIFilter::MarkerAOIFilter(const float factor) {
	factor_ = factor;
	haveMarker = false;
	markers_ = std::vector<aruco::Marker>();
	markers = &markers_;
	aois = std::vector<Circle>();
	cutoff = cv::Rect(0, 0, 0, 0);
}

/**
 * Destructor
 */
dsa::MarkerAOIFilter::~MarkerAOIFilter() {
}

void dsa::MarkerAOIFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	haveMarker = false;
	aois.clear();
}

/**
 * Fill internal data structure with markers
 */
void dsa::MarkerAOIFilter::evaluate() {
	haveMarker = markers->size() > 0;
	if(haveMarker) {
		int minX = src->cols-1;
		int minY = src->rows-1;
		int maxX = 0;
		int maxY = 0;
		for(size_t i = 0; i < markers->size(); i++) {	// for all markers
			aruco::Marker marker = (*markers)[i];
			Circle aoi;
			aoi.center = marker.getCenter();
			aoi.radius = 0;
			for(size_t i = 0; i < marker.size(); i++) {
				int radius = idistance(aoi.center, marker[i]);
				if(radius > aoi.radius)
					aoi.radius = radius;
			}
			aoi.radius *= factor_;
			aois.push_back(aoi);

			cv::Point r = cv::Point(aoi.radius, aoi.radius);	// (r,r)
			cv::Point ul = aoi.center - r;	// upper left corner of the bounding rectangle of our aoi
			cv::Point lr = aoi.center + r;	// lower right corner of the bounding rectangle of our aoi

			if(ul.x < minX)	// expand to the left
				minX = ul.x;
			if(ul.y < minY)	// expand to the top
				minY = ul.y;
			if(lr.x > maxX)	// epand to the right
				maxX = lr.x;
			if(lr.y > maxY)	// expand to the bottom
				maxY = lr.y;

			minX = std::max(minX, 0);
			minY = std::max(minY, 0);
			maxX = std::min(maxX, src->cols-1);
			maxY = std::min(maxY, src->rows-1);

			cutoff = cv::Rect(minX, minY, maxX-minX, maxY-minY);
		}
	} else	// no cutoff available, use entire image
		cutoff = cv::Rect(0, 0, src->cols-1, src->rows-1);
}

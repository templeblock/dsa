/*
 * RotationCalcs.hpp
 *
 *  Created on: 12.03.2013
 *      Author: Bigontina
 */

#ifndef ROTATIONCALCS_HPP_
#define ROTATIONCALCS_HPP_

#include <opencv2/opencv.hpp>
#include <consts.hpp>

using namespace std;
using namespace cv;

/*
 * Reorders a set of rotations in a way that
 * an average or an weighted average can be calculated.
 * The basic idea is to find the biggest gap in the cycle.
 */
void rearrangeRotations(vector<float> & rots) {
	// ensure that every rot is between 0 and 2*PI
	float pi2 = (float)(M_PI*2);
	for(int i=0;i<rots.size();i++) {
		while(rots[i]<0) rots[i] += pi2;
		while(rots[i]>=pi2) rots[i] -= pi2;
	}

	//init gaps
	vector<float> gaps;
	for(int i=0; i<rots.size(); i++) {
		gaps.push_back(-1);
	}

	// calc gaps, gaps[i] is gap after rots[i]
	for(int i=0; i<gaps.size(); i++) {
		for(int j=0;j<rots.size(); j++){
			if(i!=j) {
				float gap = rots[j] - rots[i];
				if(gap<0 || (gap == 0 && i<j)) gap += pi2;
				if(abs(gap)<1) gap = 0; // only consider major gaps, otherwise numerical or rounding errors might destroy everything
				if(gaps[i] == -1 || gaps[i] > gap) {
					gaps[i] = gap;
				}
			}
		}
	}

	//find biggest gap
	float biggestGap = 0;
	int biggestIdx = -1;
	for(int i=0;i<gaps.size();i++) {
		if(biggestGap < gaps[i]) {
			biggestGap = gaps[i];
			biggestIdx = i;
		}
	}

	if(biggestIdx == -1) {// no biggestGap found (all are equal)
		return;
	}

	// move everything after the big gap
	for(int i=0;i<rots.size();i++) {
		if(rots[i] > rots[biggestIdx]) {
			rots[i] -= pi2;
		}
	}
}

void rearrangeRotations(vector<Mat> & rvecs) {
	vector<float> xrots, yrots, zrots;
	for(int i=0;i<rvecs.size();i++) {
		xrots.push_back(rvecs[i].at<float>(0));
		yrots.push_back(rvecs[i].at<float>(1));
		zrots.push_back(rvecs[i].at<float>(2));
	}
	rearrangeRotations(xrots);
	rearrangeRotations(yrots);
	rearrangeRotations(zrots);
	for(int i=0;i<rvecs.size();i++) {
		rvecs[i].at<float>(0) = xrots[i];
		rvecs[i].at<float>(1) = yrots[i];
		rvecs[i].at<float>(2) = zrots[i];
	}
}

void rearrangeRotations(Mat r1, Mat r2) {
	vector<Mat> rots;
	rots.push_back(r1);
	rots.push_back(r2);
	rearrangeRotations(rots);
}

void normalizeRotation(float & rot) {
	float pi2 = (float)M_PI*2;
	while(rot > (float)M_PI)
		rot -= pi2;
	while(rot < -(float)M_PI)
		rot += pi2;
}

void normalizeRotation(Mat & rvec) {
	for(int i=0;i<3;i++) {
		normalizeRotation(rvec.at<float>(i));
	}
}

void normalizeRotation(vector<float> & rots) {
	for(int i=0;i<rots.size();i++)
		normalizeRotation(rots[i]);
}

float rotDist(float r1, float r2) {
	normalizeRotation(r1);
	normalizeRotation(r2);
	float diff = (r2-r1);
	normalizeRotation(diff);
	return(diff);
}


#endif /* ROTATIONCALCS_HPP_ */

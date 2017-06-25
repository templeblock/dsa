/*
 * PPP.hpp
 *
 *  Created on: 23.03.2013
 *      Author: Bigontina
 */


#ifndef PPP_HPP_
#define PPP_HPP_

#include <opencv2/opencv.hpp>
#include <RotationCalcs.hpp>

using namespace cv;

/*
 * Pose from Planar Parallel targets
 * Estimates the pose based on the assumption that
 * the object is planar, and parallel to image plane
 * of the camera.
 * Under this constraints the problem is simplified
 * and a closed form solution can be computed.
 */
class PPP {

	float getSquareSize(Mat & points) {
		Mat prev = points.col(3);

		float distsum = 0;
		for(int i=0;i<points.cols;i++) {
			distsum += dist2f(points.col(i)- prev);
			prev = points.col(i);
		}

		return(distsum/(float)points.cols);
	}

	void getSquareCenter(Mat & points, Mat & center) {
		Mat sum = Mat::zeros(2,1,CV_32FC1);
		for(int i=0;i<points.cols;i++) {
			sum += points.col(i);
		}
		center = sum/points.cols;
	}

	float getRotation(Mat & points) {
		Mat prev = points.col(0);

		float baserot = 0;
		vector<float> angles;
		for(int i=1;i<points.cols+1;i++) {
			Mat A = prev;
			Mat B = points.col(i%points.cols);

			float x = B.at<float>(0) - A.at<float>(0);
			float y = A.at<float>(1) - B.at<float>(1);
			float r = sqrt(x*x+y*y);

			// calculates the rotation clockwise, zero degree beeing at 12:00
			if(r==0) r = 1e-10;
			float angle = M_PI/2 - asin(y/r);
			if(x<0) angle = M_PI*2 - angle;

			angle = angle - M_PI/2;
			angle += baserot;
			angles.push_back(angle);

			baserot += M_PI/2.;
			prev = B;
		}

		rearrangeRotations(angles);
		float sum = 0;
		for(int i=0;i<angles.size();i++) {
			sum += angles[i];
		}
		float angle = sum/angles.size();
		normalizeRotation(angle);

		return angle;
	}

	float getDistance(float realSize, float pixelSize, Mat intrinsics) {
		float f = (intrinsics.at<float>(0,0) + intrinsics.at<float>(1,1))/2.;

		//realSize/pixelSize == dist/f
		return (realSize/pixelSize * f);
	}

	void getPosition(Mat point, float distance, Mat & intrinsics, Mat & position) {
		float f = (intrinsics.at<float>(0,0) + intrinsics.at<float>(1,1))/2.;

		float cx = intrinsics.at<float>(0,2);
		float cy = intrinsics.at<float>(1,2);

		float xpx = point.at<float>(0) - cx;
		float ypx = point.at<float>(1) - cy;

		float x = (distance/f * xpx);
		float y = (distance/f * ypx);

		position = Mat(3,1,CV_32FC1);
		position.at<float>(0) = x;
		position.at<float>(1) = y;
		position.at<float>(2) = distance;
	}

public:

	void getPose(Mat & points, float realSize, Mat intrinsics, Mat & position, Mat & rotation) {
		float zrotation = getRotation(points);
		float pixelSize = getSquareSize(points);
		float z = getDistance(realSize, pixelSize, intrinsics);

		Mat center;
		getSquareCenter(points, center);
		getPosition(center, z, intrinsics, position);

		rotation = Mat::zeros(3,1,CV_32FC1);
		rotation.at<float>(2) = zrotation;
	}

};


#endif /* PPP_HPP_ */

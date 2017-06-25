/*
 * 3dutils.hpp
 *
 *  Created on: 20.02.2013
 *      Author: Bigontina
 */

#ifndef UTILS3D_HPP_
#define UTILS3D_HPP_

#include <opencv2/opencv.hpp>
#include "utils.hpp"
#include <opencv2/imgproc/types_c.h>
using namespace cv;

/*
 * Provides useful functions for transformations and projection in 3d space
 */


void generateIntrinsicsMatrix(float fx, float fy, float cx, float cy, Mat & K) {
	K = Mat::eye(3,3,CV_32FC1);
	K.at<float>(0,0) = fx;
	K.at<float>(1,1) = fy;
	K.at<float>(0,2) = cx;
	K.at<float>(1,2) = cy;
}

void generateProjectionMatrix(Mat rvec, Mat tvec, Mat K, Mat & P, bool moveCamera=false) {
	Mat T, R, t;
	P = Mat::zeros(3, 4, CV_32FC1);
	T = Mat::zeros(3, 4, CV_32FC1);
	cv::Rodrigues(rvec, R);
	if(moveCamera) {
		T.colRange(0,3) = Mat::eye(3, 3, CV_32FC1);
		T.col(3) = tvec*(-1);
		P = K*R.t()*T;
	} else {
		T.colRange(0,3) = R;
		T.col(3) = tvec;
		P = K*T;
	}
}

void generateTranformationMatrix(Mat & rvec, Mat & tvec, Mat & T) {
	Mat R, tmp;
	T = Mat::eye(4,4, CV_32FC1);
	tmp = T.colRange(0,3).rowRange(0,3);
	cv::Rodrigues(rvec, tmp);
	tmp = T.colRange(3,4).rowRange(0,3);
	tvec.copyTo(tmp);
}

void project(Mat M, Mat points3, Mat & points2) {
	Mat o3h, i3;
	addOnesRow(points3, o3h);
	i3 = M * o3h;
	normHomogeneous2(i3, points2, true);
}

void transform3d(Mat T, Mat srcpoints3, Mat & dstpoints3) {
	Mat src3h, dst4;
	addOnesRow(srcpoints3, src3h);
	dst4 = T * src3h;
	normHomogeneous2(dst4, dstpoints3, true);
}

void generateFlipMatrix(int imgheight, Mat & F) {
	F = Mat::eye(3, 3, CV_32FC1);
	F.at<float>(1,2) = imgheight;
	F.at<float>(1,1) = -1;
}

void warpImage(Mat & image, Mat & H, Mat & dstimg) {
	if(image.channels() != 3) {
		cout << "ATTENTION: converting image (in warpImage)" << endl;
		cv::cvtColor(image, image, CV_GRAY2RGB);
		image.convertTo(image, CV_8UC3);
	}
	if(dstimg.channels() != 3) {
		cout << "ATTENTION: converting dstimg (in warpImage)" << endl;
		cv::cvtColor(dstimg, dstimg, CV_GRAY2RGB);
	}

	Mat cornersH,corners;
	Mat cornersM = Mat(3,4,CV_32FC1);
	fillMat3f(0, 0, 1, cornersM,0);
	fillMat3f(0, image.cols-1, 1, cornersM,1);
	fillMat3f(image.rows-1, image.cols-1, 1, cornersM,2);
	fillMat3f(image.rows-1, 0, 1, cornersM,3);
	cornersH = H*cornersM;
	normHomogeneous2(cornersH, corners, true);

	Mat Hinv = H.inv();
	double xmin, xmax, ymin, ymax;
	cv::minMaxLoc(corners.row(0), &xmin, &xmax);
	cv::minMaxLoc(corners.row(1), &ymin, &ymax);
	xmin-=10;
	ymin-=10;
	xmax+=10;
	ymax+=10;
	if(xmin<0) xmin = 0;
	if(xmax>=dstimg.cols) xmax = dstimg.cols-1;
	if(ymin<0) ymin = 0;
	if(ymax>=dstimg.rows) ymax = dstimg.rows-1;

	Mat p1,p2;
	int x2,y2;
	Mat ps = Mat(3,4,CV_32FC1);
	for(int x=xmin;x<xmax;x++) {
		for(int y=ymin;y<ymax;y++) {
			fillMat3f(x-0.25,y-0.25,1,ps,0);
			fillMat3f(x+0.25,y-0.25,1,ps,1);
			fillMat3f(x+0.25,y+0.25,1,ps,2);
			fillMat3f(x-0.25,y+0.25,1,ps,3);
			p2 = Hinv*ps;
			Vec3i sum = 0;
			int count = 0;
			for(int i=0;i<4;i++) {
				x2 = round(p2.at<float>(0,i)/p2.at<float>(2,i));
				y2 = round(p2.at<float>(1,i)/p2.at<float>(2,i));
				if(0<=x2 && x2<image.cols && 0<=y2 && y2<image.rows) {
					Vec3i v = image.at<Vec3b>(y2,x2);
					sum += v;
					count++;
				}
			}
			if(count>0) {
				dstimg.at<Vec3b>(y,x) = sum*(1.0/count);
			}
		}
	}
}


#endif /* UTILS3D_HPP_ */


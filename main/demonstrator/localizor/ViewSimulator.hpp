
#ifndef VIEWSIMULATOR_HPP_
#define VIEWSIMULATOR_HPP_

#include "opencv2/opencv.hpp"
#include <utils.hpp>
#include <Settings.hpp>
#include <aruco/arucofidmarkers.h>
#include <utils3d.hpp>
#include <MarkerUtils.hpp>


using namespace cv;


/*
 * Generates a virtual view of the marker in a
 * certain pose.
 */
class ViewSimulator {


public:

	void imageCoordsHomo(int width, int height, Mat & H) {
		H = Mat(3,3,CV_32FC1);
		H.at<float>(0,0) = width/2.;
		H.at<float>(1,1) = height/2.;
		H.at<float>(0,2) = width/2.;
		H.at<float>(1,2) = height/2.;
	}

	void normImageHomo(int width, int height, float markerSize, Mat & H) {
		H = Mat::eye(3,3,CV_32FC1);
		H.at<float>(0,0) = markerSize/(width-1);
		H.at<float>(1,1) = markerSize/(height-1);
		H.at<float>(0,2) = -markerSize/2;
		H.at<float>(1,2) = -markerSize/2;
	}

	void projectMarker(float markersize, Mat rvec, Mat tvec, Mat K, Mat & projectedPoints) {
		Mat P,x,y,z;
		generateProjectionMatrix(rvec, tvec, K, P);
		genMarkerPoints(markersize, x, true);
		y = P*x;
		normHomogeneous2(y, projectedPoints, true);
	}

	void addNoise(Mat & img, float howmuch) {
		Mat noise = Mat(img.rows, img.cols, img.type());
		randu(noise, 0, 255);
		img = img*(1-howmuch)+noise*howmuch;
	}

	void reduceContrast(Mat & img, float contrast, int center=128) {
		img = (img-center)*contrast+center;
	}

	void warpMarker(Mat marker, Mat H, Mat corners, Mat dstimg) {
		Mat Hinv = H.inv();
		double xmin, xmax, ymin, ymax;
		cv::minMaxLoc(corners.row(0), &xmin, &xmax);
		cv::minMaxLoc(corners.row(1), &ymin, &ymax);
		xmin-=100;
		ymin-=100;
		xmax+=100;
		ymax+=100;
		if(xmin<0) xmin = 0;
		if(xmax>=dstimg.cols) xmax = dstimg.cols-1;
		if(ymin<0) ymin = 0;
		if(ymax>=dstimg.rows) ymax = dstimg.rows-1;
		//TODO: check dst boundaries!
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
				int sum = -1;
				for(int i=0;i<4;i++) {
					x2 = round(p2.at<float>(0,i)/p2.at<float>(2,i));
					y2 = round(p2.at<float>(1,i)/p2.at<float>(2,i));
					if(0<=x2 && x2<marker.cols && 0<=y2 && y2<marker.rows) {
						sum += marker.at<uint8_t>(y2,x2);
					}
				}
				if(sum>-1) {
					dstimg.at<uint8_t>(y,x) = sum/4;
				}
			}
		}
	}

	void generateView(Mat markerimg, float markersize, Mat rcam, Mat tcam, Mat robj, Mat tobj, Mat K, Size dstsize, Mat & dstimg) {
		Mat P,T;

		Mat opoints3h, ipoints2, opoints2;

		generateTranformationMatrix(robj, tobj, T);
		generateProjectionMatrix(rcam, tcam, K, P, true);
		genMarkerPoints(markersize, opoints3h, true);

		project(P*T, opoints3h, ipoints2);
		opoints2 = opoints3h.rowRange(0,2);

		Mat H = findHomography(opoints2.t(), ipoints2.t());
		H.convertTo(H, CV_32FC1);
		Mat imageH;
		normImageHomo(markerimg.cols, markerimg.rows, markersize, imageH);

		Mat newH = H*imageH;

		dstimg = Mat::zeros(dstsize, markerimg.type());

		//warpPerspective(markerimg, dstimg, newH, dstsize, INTER_NEAREST, BORDER_TRANSPARENT);
		//warpMarker(markerimg, newH, opoints2, dstimg);

		float noise = 0;//0.2;
		if(noise>0) {
			addNoise(dstimg,noise);
		}

		float contrast = 0;//0.8;
		if(contrast>0) {
			reduceContrast(dstimg, contrast);
		}
	}

	void testSim() {
		Mat tcam, rcam, tobj, robj, K;
		Mat marker = imread("markers/marker-0022.bmp");
		imshow("marker", marker);

		toMat3f(0., 0., 0., tcam);
		toMat3f(0., 0., 0., rcam);

		toMat3f(0., 0., 10., tobj);
		toMat3f(0., 0., 0., robj);

		Size s = Size(640,480);
		generateIntrinsicsMatrix(700,700,320,240,K);
		Mat img;
		generateView(marker, 1, rcam, tcam, robj, tobj, K, s, img);
		imshow("sim", img);
		waitKey();
	}
};

#endif /* VIEWSIMULATOR_HPP_ */

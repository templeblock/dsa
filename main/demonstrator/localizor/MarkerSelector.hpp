/*
 * MarkerInfo.hpp
 *
 *  Created on: Jan 8, 2013
 *      Author: Bigontina
 */

#ifndef MARKERSELECTOR_HPP_
#define MARKERSELECTOR_HPP_

#include <math.h>
#include <aruco/arucofidmarkers.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;


/*
 * This class is used to compare different markers,
 * to be able to choose the ones which can be distinguished
 * the best from each other.
 * This class was used in a preliminary step to choose good markers.
 * It is however not used for the actual localization.
 */
class MarkerSelector {
public:

	int hammingDist(Mat & m1, Mat & m2) {
		int sum = 0;
		for(int i=0;i<m1.rows;i++) {
			for(int j=0;j<m1.cols;j++) {
				sum += abs(m1.at<uint8_t>(i,j)-m2.at<uint8_t>(i,j));
			}
		}
		return sum;
	}

	int variationScore(Mat & m) {
		int var = 0;
		for(int i=0;i<m.rows;i++) {
			for(int j=0;j<m.cols;j++) {
				var += m.at<uint8_t>(i,j)*2-1;
			}
		}
		return m.cols*m.rows-abs(var);
	}

	int hammingDistSum(vector<Mat> ms, Mat n) {
		int sum = 0;
		for(int i=0;i<ms.size();i++) {
			sum += hammingDist(ms[i], n);
		}
		return sum;
	}

	Mat rotateMarker(Mat m) {
		Mat r = Mat(m.rows,m.cols,m.type());
		for(int x=0;x<m.cols;x++) {
			for(int y=0;y<m.rows;y++) {
				r.at<uint8_t>(x,m.cols-1-y) = m.at<uint8_t>(y,x);
			}
		}
		return r;
	}

	int worstHammingDist(vector<Mat> ms, Mat n) {
		int lowestham = 10000;
		for(int i=0;i<ms.size();i++) {
			Mat m = ms[i];
			for(int j=0;j<4;j++) {
				int ham = hammingDist(m, n);
				if(ham < lowestham) {
					lowestham = ham;
				}
				m = rotateMarker(m);
			}
		}
		return(lowestham);
	}

	int bestHammingDist(vector<Mat> ms) {
		int bestsum=-1;
		int bestid=0;
		for(int id=0;id<1024;id++) {
			Mat n = aruco::FiducidalMarkers::getMarkerMat(id);
			int sum = worstHammingDist(ms, n);
			if(sum>bestsum) {
				bestsum = sum;
				bestid = id;
			}
		}
		return bestid;
	}

	int findNiceMarker() {
		int bestscore = -1;
		int bestid = -1;
		for(int id=0;id<1024;id++) {
			Mat m = aruco::FiducidalMarkers::getMarkerMat(id);
			int score = variationScore(m);
			if(score>bestscore) {
				bestscore = score;
				bestid = id;
			}
		}
		return bestid;
	}

	void showMarker(int id) {
		Mat img = aruco::FiducidalMarkers::createMarkerImage(id,700);
		namedWindow("marker", CV_WINDOW_AUTOSIZE);
		cv::imshow("marker", img);
		cv::waitKey(0);
		img = rotateMarker(img);
		namedWindow("marker", CV_WINDOW_AUTOSIZE);
		cv::imshow("marker", img);
		cv::waitKey(0);
	}

	vector<int> bestMarkerSorting(int count) {
		vector<int> ids;
		vector<Mat> ms;
		int id = findNiceMarker();
		showMarker(id);
		//return(0);
		Mat m = aruco::FiducidalMarkers::getMarkerMat(id);
		ids.push_back(id);
		ms.push_back(m);
		while(ms.size()<count) {
			id = bestHammingDist(ms);
			m = aruco::FiducidalMarkers::getMarkerMat(id);
			ids.push_back(id);
			ms.push_back(m);
		}

		cout << "start ids" << endl;
		for(int i=0;i<ids.size();i++) {
			cout << ids[i] << endl;
		}
		return(ids);
	}

	void test() {
		ostringstream strstream;
		vector<int> markerIds = bestMarkerSorting(25);
		for(int i=0;i<markerIds.size();i++) {
			strstream.str("");
			strstream << "markers/" << "marker-";
			strstream.fill('0');
			strstream.width(4);
			strstream << markerIds[i] << ".bmp";
			Mat img = aruco::FiducidalMarkers::createMarkerImage(markerIds[i], 700);
			imwrite(strstream.str(),img);
		}
	}

};

#endif /* MARKERSELECTOR_HPP_ */

/*
 * Test_Localizor.hpp
 *
 *  Created on: 24.03.2013
 *      Author: user
 */

#ifndef TEST_LOCALIZOR_HPP_
#define TEST_LOCALIZOR_HPP_

#include <opencv2/opencv.hpp>
#include <utils3d.hpp>
#include <aruco/cameraparameters.h>

using namespace cv;

class TestLocalizor {

	void projectMarker(Mat & P, MarkerPose & pose, Marker & m, float detectionError = 0.0) {
		Mat markerPoints;
		genMarkerPoints(pose.size, markerPoints,false);

		Mat T;
		generateTranformationMatrix(pose.rotation, pose.position, T);

		Mat imagePoints;
		project(P*T, markerPoints, imagePoints);

		if(detectionError > 0) {
			Rnd rnd = Rnd();
			for(int i=0;i<imagePoints.cols;i++) {
				imagePoints.at<float>(0,i) += rnd.next(-detectionError, detectionError);
				imagePoints.at<float>(1,i) += rnd.next(-detectionError, detectionError);
			}
		}

		vector<Point2f> pointsVec;
		mat2fToPoints(imagePoints, pointsVec);
		m = Marker(pointsVec, pose.id);
	}

public:

	void test() {
		float markerSize = 10;

		Mat pos1, rot1;
		toMat3f(-50,-50,400,pos1);
		toMat3f(0,0,0,rot1);
		MarkerPose pose = MarkerPose(1, pos1, rot1, markerSize);

		vector<Mat> tvecs, rvecs;

		int count = 10;
		for(int i=0;i<count;i++) {

			Mat tcam, rcam;
			toMat3f(0,0,0,tcam);
			toMat3f(0,0,M_PI*2*(i/(float)count),rcam);


			Mat K;
			generateIntrinsicsMatrix(900, 900, 384, 288, K);
			Mat distortion = Mat::zeros(4,1,CV_32FC1);
			Size camSize = Size(768, 576);
			CameraParameters camParams = CameraParameters(K, distortion, camSize);

			// create projection matrix
			Mat F, P;
			K = camParams.CameraMatrix;
			generateFlipMatrix(camParams.CamSize.height, F);
			generateProjectionMatrix(rcam, tcam, F*K, P, true);

			Marker marker;
			projectMarker(P, pose, marker, 0);


			Mat tvec, rvec;
			Localizor l = Localizor(camParams, PoseEstimationAlgorithm::ITERATIVE, RegulationStrategy::PARALLELISM);
			l.estimatePose(marker, pose, tvec, rvec);

			tvecs.push_back(tvec);
			rvecs.push_back(rvec);

			cout << " ---------- " << endl;
			cout << "tvec: " << tvec << endl;
			cout << "rvec: " << rvec << endl;
		}

		cout << endl << "Summary: "<< endl;
		for(int i=0;i<count;i++) {
			cout << " ---------- " << endl;
			cout << "tvec: " << tvecs[i] << endl;
			cout << "rvec: " << rvecs[i] << endl;
		}
		cout << " ---------- " << endl;
		for(int i=0;i<count;i++) {
			cout << "zrot " << i << ": " << rvecs[i].at<float>(2) << endl;
		}
	}

};


#endif /* TEST_LOCALIZOR_HPP_ */

/*
 * Test_PPP.hpp
 *
 *  Created on: 23.03.2013
 *      Author: user
 */

#ifndef TEST_PPP_HPP_
#define TEST_PPP_HPP_

#include <opencv2/opencv.hpp>
#include <PPP.hpp>
#include <MarkerUtils.hpp>

using namespace cv;
using namespace std;

class TestPPP{
public:
	void test() {
		int count = 100;
		vector<Mat> tvecs, rvecs;

		for(int i=0;i<count;i++) {
			float angle = M_PI*2*(i/(float)count);

			cout << " ======== "<< endl;
			cout << " = Angle: " << angle << endl;

			Mat tvec,rvec;
			test(angle, tvec, rvec);
			tvecs.push_back(tvec);
			rvecs.push_back(rvec);
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

	void test(float zrot, Mat & tvec, Mat & rvec) {
		Mat model3, image2;

		float markerSize = 1;

		genMarkerPoints(markerSize, model3, false);
//		genCubePoints(1, model3);


		Mat T;
		Mat r, t;
		toMat3f(0,0,zrot,r);
		toMat3f(0,0,4,t);
		generateTranformationMatrix(r,t,T);
		transform3d(T, model3, model3);

		Mat K, F;
		generateIntrinsicsMatrix(900, 900, 384, 288, K);
		//generateFlipMatrix(576,F); // NO FLIP NECESSARY CAUSE 'unflipping' is only done in localizor

		Mat P;
		Mat rcam, tcam;
		toMat3f(0,0,0,rcam);
		toMat3f(0,0,-2,tcam);
		generateProjectionMatrix(rcam, tcam, K, P, true);

		project(P, model3, image2);


		PPP ppp = PPP();
		ppp.getPose(image2, markerSize, K, tvec, rvec);

	}

};


#endif /* TEST_PPP_HPP_ */

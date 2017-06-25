/*
 * Test_PoseEstimator.hpp
 *
 *  Created on: 18.03.2013
 *      Author: user
 */

#ifndef TEST_POSEESTIMATOR_HPP_
#define TEST_POSEESTIMATOR_HPP_

#include <MarkerUtils.hpp>

using namespace std;

class TestPoseEstimator{
public:
	void genCubePoints(float size, Mat & model) {
		model = Mat::zeros(3, 8, CV_32FC1);
		Mat modelFront;
		Mat modelBack;

		genMarkerPoints(size, modelFront, false);
		genMarkerPoints(size, modelBack, false);

		Mat T;
		Mat r, t;
		toMat3f(0,0,0,r);
		toMat3f(0,0,size,t);
		generateTranformationMatrix(r,t,T);
		transform3d(T, modelBack, modelBack);

		modelFront.copyTo(model.colRange(0,4));
		modelBack.copyTo(model.colRange(4,8));
	}

	void test() {
		Mat model3, image2;

		genMarkerPoints(1, model3, false);
//		genCubePoints(1, model3);

		model3.at<float>(2,0) += 0.01;

		Mat T;
		Mat r, t;
		toMat3f(0,0,0,r);
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

		cout << endl;
		cout << "model3:" << endl << model3 << endl;
		cout << "image2:" << endl << image2 << endl;

		cout << "--------------------" << endl;

		testIterative(K, model3, image2);
		cout << "--------------------" << endl;

		testEPNP(K, model3, image2);
		cout << "--------------------" << endl;

		testP3P(K, model3, image2);
		cout << "--------------------" << endl;

		testRPP(K, model3, image2);
		cout << "--------------------" << endl;

		testPosit(K, model3, image2);
		cout << "--------------------" << endl;

		testPlanarPosit(K, model3, image2);
		cout << "--------------------" << endl;
	}

	void testIterative(Mat & K, Mat model3, Mat image2) {
		Mat tvec, rvec;
		PoseEstimator estimator = PoseEstimator(K, PoseEstimationAlgorithm::ITERATIVE);
		estimator.getPose(model3, image2, tvec, rvec);
		cout << " > tvec: " << tvec << endl;
		cout << " > rvec: " << rvec << endl;
	}

	void testEPNP(Mat & K, Mat model3, Mat image2) {
		Mat tvec, rvec;
		PoseEstimator estimator = PoseEstimator(K, PoseEstimationAlgorithm::EPNP);
		estimator.getPose(model3, image2, tvec, rvec);
		cout << " > tvec: " << tvec << endl;
		cout << " > rvec: " << rvec << endl;
	}

	void testP3P(Mat & K, Mat model3, Mat image2) {
		Mat tvec, rvec;
		PoseEstimator estimator = PoseEstimator(K, PoseEstimationAlgorithm::P3P);
		estimator.getPose(model3, image2, tvec, rvec);
		cout << " > tvec: " << tvec << endl;
		cout << " > rvec: " << rvec << endl;
	}

	void testRPP(Mat & K, Mat model3, Mat image2) {
		Mat tvec, rvec;
		PoseEstimator estimator = PoseEstimator(K, PoseEstimationAlgorithm::RPPAlgo);
		estimator.getPose(model3, image2, tvec, rvec);
		cout << " > tvec: " << tvec << endl;
		cout << " > rvec: " << rvec << endl;
	}

	void testPosit(Mat & K, Mat model3, Mat image2) {
		Mat tvec, rvec;
		PoseEstimator estimator = PoseEstimator(K, PoseEstimationAlgorithm::POSIT);
		estimator.getPose(model3, image2, tvec, rvec);
		cout << " > tvec: " << tvec << endl;
		cout << " > rvec: " << rvec << endl;
	}

	void testPlanarPosit(Mat & K, Mat model3, Mat image2) {
		Mat tvec, rvec;
		PoseEstimator estimator = PoseEstimator(K, PoseEstimationAlgorithm::PLANARPOSIT);
		estimator.getPose(model3, image2, tvec, rvec);
		cout << " > tvec: " << tvec << endl;
		cout << " > rvec: " << rvec << endl;
	}

};


#endif /* TEST_POSEESTIMATOR_HPP_ */

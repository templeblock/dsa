/*
 * Test_utils3d.hpp
 *
 *  Created on: 04.03.2013
 *      Author: user
 */

#ifndef TEST_UTILS3D_HPP_
#define TEST_UTILS3D_HPP_

#include <utils3d.hpp>
#include <utils.hpp>

class TestUtils3d {
public:
	void test() {
		Mat K, P, T, rcam, tcam, robj, tobj, object, ipoints;
		float fx, fy, cx, cy;

		fx = 1400;
		fy = 1400;
		cx = 640;
		cy = 480;

		toMat3f(0, 0, -300, tcam);
		toMat3f(0, 0, 0, rcam);

		toMat3f(0, 10, 0, tobj);
		toMat3f(0, 0, 0, robj);

		object = Mat(3, 3, CV_32FC1);
		fillMat3f(-5, 5, 0, object, 0);
		fillMat3f( 5, 5, 0, object, 1);
		fillMat3f(-5,-5, 0, object, 2);

		generateIntrinsicsMatrix(fx, fy, cx, cy, K);
		generateProjectionMatrix(rcam, tcam, K, P, true);
		generateTranformationMatrix(robj, tobj, T);
		Mat F;
		generateFlipMatrix(960, F);

		project(F*P*T, object, ipoints);


		cout << "object:  " << object << endl;
		cout << "ipoints: " << ipoints << endl;
	}
};

#endif /* TEST_UTILS3D_HPP_ */

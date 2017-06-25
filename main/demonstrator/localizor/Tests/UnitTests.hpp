/*
 * tests.hpp
 *
 *  Created on: Jan 8, 2013
 *      Author: bigo
 */

#ifndef UNITTESTS_HPP_
#define UNITTESTS_HPP_


#include <opencv2/opencv.hpp>
#include <utils.hpp>

#include <Tests/Test_utils3d.hpp>
#include <Tests/Test_pose.hpp>
#include <Tests/Test_PoseEstimator.hpp>
#include <Tests/Test_PPP.hpp>
#include <Tests/Test_Localizor.hpp>
#include <Tests/Test_RotationCalcs.hpp>

using namespace cv;
using namespace std;



void unitTests() {
//	TestUtils3d* t  = new TestUtils3d();
//	t->test();

//	TestPoseEstimator* t = new TestPoseEstimator();
//	t->test();

//	TestPose* p = new TestPose();
//	p->test();

	TestRotationCalcs t = TestRotationCalcs();
	//t.test();
	t.testAveraging();

//	TestPPP* t = new TestPPP();
//	t->test();

//	TestLocalizor t = TestLocalizor();
//	t.test();

}




#endif /* UNITTESTS_HPP_ */

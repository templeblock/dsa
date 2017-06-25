/*
 * Test_pose.hpp
 *
 *  Created on: 04.03.2013
 *      Author: user
 */

#ifndef TEST_POSE_HPP_
#define TEST_POSE_HPP_

#include <Scene.hpp>
#include <opencv2/opencv.hpp>
#include <MarkerGeneration.hpp>
#include <MarkerUtils.hpp>

using namespace cv;

class TestPose {
public:
	void drawMarkerImages(Scene & scene, vector<MarkerPose> & poses) {
		for(int i=0;i<poses.size();i++) {
			MarkerPose p = poses[i];
			Mat markerimg = createMarker(p.id);
			scene.drawMarkerImage(markerimg, p.size, p.position, p.rotation);
		}
	}

	void drawMarkerWires(Scene & scene, vector<MarkerPose> & poses) {
		for(int i=0;i<poses.size();i++) {
			MarkerPose p = poses[i];
			ostringstream text; text<< p.id;
			scene.drawMarkerWire(p.size, p.position, p.rotation, text.str());
		}
	}

	float rad(float deg) {
		return(M_PI/180.*deg);
	}

	void test() {
		Mat tcam, rcam, tobj, robj, K;

		float markerSize = 10;

		toMat3f(0., 0., -200., tcam);
		toMat3f(rad(0.), 0., 0., rcam); //M_PI/180.*45;

		vector<MarkerPose> poses;
		toMat3f(50., 0., 0., tobj);
		toMat3f(rad(0.), 0., 0., robj);
		MarkerPose p = MarkerPose(32, tobj, robj, markerSize);
		poses.push_back(p);
		toMat3f(-50., 50., 0., tobj);
		toMat3f(0., rad(0.), 0., robj);
		p = MarkerPose(22, tobj, robj, markerSize);
		//		poses.push_back(p);

		Size s = Size(1024,768);

		//Settings settings = Settings();
		//settings.readCameraParameters(s);
		Mat F;
		generateFlipMatrix(s.height, F);

		generateIntrinsicsMatrix(s.width*1.1,s.width*1.1,s.width/2,s.height/2,K);//settings.camParameters.CameraMatrix;
		//		K = F*K;
		//		Mat img;
		//		generateView(marker, 1, rcam, tcam, robj, tobj, K, s, img);

		Scene* scene = new Scene(K, tcam, rcam, s);
		//scene->drawMarkerWire(markerSize, tobj, robj);
		//		scene->drawMarkerImage(marker, markerSize, tobj, robj);
		drawMarkerImages(*scene, poses);
		drawMarkerWires(*scene, poses);
		cout << "...scene..." << endl;

		Mat screen;
		//		cvtColor(*scene->getScreen(), screen, CV_GRAY2RGB);
		scene->getScreen()->copyTo(screen);

		imshow("hallo",screen);
		waitKey();

		Mat dist = Mat::zeros(4,1,CV_32FC1);
		CameraParameters camParams = CameraParameters(K, dist, s);

		vector<Marker> markers;
		Mat t_est, r_est;

		Localizor* l = new Localizor(camParams);
		findMarkers(screen, markers, camParams);
		cout << "Marker count: " << markers.size() << endl;
		if(markers.size() == 0) {
			cout << "NO MARKERS FOUND!!!" << endl;
			exit(0);
		}
		l->estimatePose(markers, poses, t_est, r_est);

		cout << "t_est: " << t_est << endl;
		cout << "r_est: " << r_est << endl;

		//		scene->clearScreen();
		scene->setViewPose(t_est, r_est);
		drawMarkerWires(*scene, poses);

		//		cvtColor(*scene->getScreen(), screen, CV_GRAY2RGB);
		scene->getScreen()->copyTo(screen);
		imshow("hallo",screen);
		waitKey();


		Mat P;
		generateProjectionMatrix(rcam, tcam, K, P, true);

		Mat p1, p2;

		toMat3f(-55, -5, 0, p1);
		addOnesRow(p1, p1);
		cout << endl;
		cout << "p1" << " " << p1 << endl;
		p2 = P*p1;
		normHomogeneous2(p2, p2, TRUE);
		cout << "p2" << " " << p2 << endl;

		toMat3f(-55, 55, 0, p1);
		addOnesRow(p1, p1);
		cout << endl;
		cout << "p1" << " " << p1 << endl;
		p2 = P*p1;
		normHomogeneous2(p2, p2, TRUE);
		cout << "p2" << " " << p2 << endl;
	}
};

#endif /* TEST_POSE_HPP_ */

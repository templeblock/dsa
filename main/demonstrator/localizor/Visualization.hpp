/*
 * Visualization.hpp
 *
 *  Created on: 02.03.2013
 *      Author: Bigontina
 */

#ifndef VISUALIZATION_HPP_
#define VISUALIZATION_HPP_

#include <opencv2/opencv.hpp>
#include <ostream>

#include <Listener.hpp>
#include <utils.hpp>

using namespace cv;
using namespace std;

/*
 * Show some visualization of the current input image of the LocalizorSys
 * It shows:
 *  - A window with the current input video
 *  - A window with a 3D-Visualization of the camera position
 *  - A window with a 2D-Visualization
 */
class Visualization : Listener {

public:

	LocalizorSys* localizorSys;
	Display* display;
	Mat tvec_prev, rvec_prev;
	ostringstream fileoutput;
	int frameno;

	Visualization(LocalizorSys * locSys) {
		frameno = 0;

		display = new Display();
		display->init();

		localizorSys = locSys;
		localizorSys->addEventListener(Listener::ITERATION, this);
	}

	void eventHandler(Listener::Event event) {
		switch(event) {
		case Listener::ITERATION:
			iterationHandler();
			break;
		}
	}

	void iterationHandler() {
		Mat* img = localizorSys->getImage();
		Mat* tvec = localizorSys->getCamPosition();
		Mat* rvec = localizorSys->getCamRotation();
		int markerCount = localizorSys->getMarkerCount();
		CameraParameters params = localizorSys->getCamParams();
		Mat K = params.CameraMatrix;
		vector<Marker> markers = localizorSys->getMarkers();
		vector<MarkerPose> poses = localizorSys->getPoses();
		Mat t,r;

		// add overlay text
		String note;
		if (markerCount>0){
			tvec_prev = *tvec;
			rvec_prev = *rvec;
			note = "";
		} else {
			note = " (no detection)";
		}
		display->setImage(img);
		display->textstream << "Rotation: " << rvec_prev << note << '\n';
		display->textstream << "Translation: " << tvec_prev << note << '\n';
		display->show();


		// create 3D visualization of camera position
		namedWindow("Visualization");
		Mat viscamt, viscamr;
		toMat3f(223, 564, -300, viscamt);
		toMat3f(0,0,0, viscamr);
		Scene vis = Scene(params.CameraMatrix, viscamt, viscamr, params.CamSize);
		for(int i=0;i<markers.size();i++) {
			poses[i].get(t,r);
			vis.drawMarkerWire(poses[i].size, t, r);
		}
		vis.drawCameraWire(*tvec, *rvec, 10, Scalar(255,255,255));
		imshow("Visualization", *vis.getScreen());


		// create 2d visualization
		Mat screen = Mat::zeros(480, 640, CV_8UC3);
		Mat orthoK = Mat::eye(3,3,CV_32FC1);
		fillMat3f(screen.cols/2., screen.rows/2., 1, orthoK, 2);
		Mat viscamt2d, viscamr2d;
		toMat3f(223, 564,-300, viscamt2d);
		toMat3f(0, 0, 0, viscamr2d);
		Scene vis2d = Scene(orthoK, viscamt, viscamr, screen, true);
		vector<MarkerPose*> allposes;
		localizorSys->markerStorage.getAll(allposes);
		for(int i=0;i<allposes.size();i++) {
			allposes[i]->get(t,r);
			vis2d.drawMarkerWire(allposes[i]->size, t, r, "", Scalar(128,128,128));
		}
		for(int i=0;i<poses.size();i++) {
			poses[i].get(t,r);
			vis2d.drawMarkerWire(poses[i].size, t, r, "", Scalar(128,256,128));
		}
		vis2d.drawCameraWire(*tvec, *rvec, 8, Scalar(0,0,255));
		imshow("Visualization2d", *vis2d.getScreen());

		char key = cv::waitKey(10);
		if(key != -1) handleKeys(key);
	}

	void handleKeys(char key) {
		ostringstream fname("");
		cout << "KEY PRESSED :: " << key << endl;
		switch(key) {
		case 'q':
			localizorSys->stop();
			break;
		case 'p':
			fname << "snapshots/snapshot_";
			fname << time(NULL) << ".jpg";
			cv::imwrite(fname.str(), *localizorSys->getImage());
			break;
		case '#':
			cv::waitKey();
			break;
		}

	}



};

#endif /* VISUALIZATION_HPP_ */

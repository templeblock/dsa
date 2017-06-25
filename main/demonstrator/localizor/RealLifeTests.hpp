/*
 * RealLifeTests.hpp
 *
 *  Created on: 15.04.2013
 *      Author: Bigontina
 */

#ifndef REALLIFETESTS_HPP_
#define REALLIFETESTS_HPP_

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
 * This class is only used to evaluate test videos.
 * It stores the pose at a certain point in time
 * and provides an interpolation function.
 */
class Pose {
public:
	float x,y;
	float r;
	float time;

	void moveTowards(Pose & B, float time, Pose & intermediatePose) {
		float percentage = (time - this->time)/(B.time - this->time);
		intermediatePose.x = this->x + (B.x-this->x)*percentage;
		intermediatePose.y = this->y + (B.y-this->y)*percentage;
		intermediatePose.r = this->r + (B.r-this->r)*percentage;
		intermediatePose.time = time;
	}

	void getPosition(Mat & p) {
		p = Mat::zeros(3,1,CV_32FC1);
		p.at<float>(0) = x;
		p.at<float>(1) = y;
	}
	void getRotation(Mat & rot) {
		rot = Mat::zeros(3,1,CV_32FC1);
		rot.at<float>(2) = r;
	}
};


/*
 * Based on config files test videos are evaluated.
 * Not used for actual system, just for test evaluation.
 */
class RealLifeTests: Listener {
public:
	float time;
	float fps;
	Pose currentTruthPose;
	vector<Pose*> keyPoses;
	Pose* prevKeyPose;
	Pose* nextKeyPose;
	int keyPoseIdx;
	LocalizorSys* localizorSys;
	vector<Pose*> estimatedPoses;
	vector<Pose*> truthPoses;
	bool done;
	string videofile;
	string outputfile;
	Mat viscamt2d, viscamr2d;
	bool ortho;
	string appendfile1;
	string appendfile2;
	string appendfile3;

	RealLifeTests(string configfile){
		time = 0;
		this->load(configfile);
		keyPoseIdx = 0;
		done = false;
		toMat3f(223, 564,1400, viscamt2d);
		toMat3f(M_PI, 0, 0, viscamr2d);
		ortho = true;
	}

	void filereplacements(string & filename) {
		replace(filename, "$m", (int)localizorSys->settings.multiMarkerStrategy);
		replace(filename, "$r", (int)localizorSys->settings.regulationStrategy);
		replace(filename, "$p", (int)localizorSys->settings.poseEstimationAlgorithm);
	}


	void run() {
		VideoCapture* capture = new VideoCapture(videofile);
		localizorSys = new LocalizorSys("versuchshalle");
		localizorSys->addEventListener(Listener::ITERATION, (Listener*)this);

		filereplacements(outputfile);

		localizorSys->run(capture);
	}

	void load(string configfile) {
		float carpetBlockSize = 12;
		float trollyWidth = 48;
		float trollyLength = 79.5;
		float camToTrollyY = 7;
		FileStorage fs(configfile, FileStorage::READ);
		float carpetOriginX = (float)fs["carpetOrigin"]["x"];
		float carpetOriginY = (float)fs["carpetOrigin"]["y"];

		this->videofile = (string)fs["videofile"];
		this->outputfile = (string)fs["outputfile"];

		//char* fnamechar, p1, p2 ,p3;
		//_splitpath(configfile.c_str(), p1, p2, fnamechar, p3);
		//string fname(fnamechar);
		//replace(videofile, "$f", fname);
		//replace(outputfile, "$f", fname);

		this->appendfile1 = (string)fs["appendfile1"];
		this->appendfile2 = (string)fs["appendfile2"];
		this->appendfile3 = (string)fs["appendfile3"];

		bool relativePoses = ((int)fs["relativePoses"] == 1);
		this->fps = (float)fs["fps"];

		FileNode posesNode = fs["keyPoses"];
		for(FileNodeIterator it = posesNode.begin(); it != posesNode.end(); it++) {
			FileNode n = (FileNode)(*it);
			Pose* p = new Pose();
			p->x = (float)n["x"];
			p->y = (float)n["y"];
			p->r = (float)n["r"];
			p->time = (float)n["time"];
			keyPoses.push_back(p);
		}

		if(relativePoses) {
			for(int i=0;i<keyPoses.size();i++) {
				float offsetx = trollyWidth/2;
				float offsety = trollyLength - camToTrollyY;

				Mat offset;
				toMat3f(offsetx, offsety, 0, offset);
				Mat rvec, rmat;
				toMat3f(0, 0, -keyPoses[i]->r, rvec);
				Rodrigues(rvec, rmat);
				offset = rmat * offset;
				offsetx = offset.at<float>(0);
				offsety = offset.at<float>(1);

				keyPoses[i]->x = carpetOriginX + keyPoses[i]->x*carpetBlockSize + offsetx;
				keyPoses[i]->y = carpetOriginY + keyPoses[i]->y*carpetBlockSize + offsety;
			}
		}

		fs.release();
	}

	float cl;
	void next() {
		float now = (float)clock();
		cl = now;

		if(done) return;
		if(time < keyPoses[keyPoseIdx]->time) {
			time += 1./fps;
			return;
		}
		if(time >= keyPoses[keyPoseIdx+1]->time) keyPoseIdx++;
		if(keyPoses.size() <= keyPoseIdx+1) {
			finish();
			exit(0);
		}

		Mat rz, rx,r;
		toMat3f(0,0,1.57,rz);
		toMat3f(3.14,0,0,rx);
		Mat matrz, matrx;
		Rodrigues(rz, matrz);
		Rodrigues(rx, matrx);
		Rodrigues(matrx*matrz, r);

		Pose* currentTruthPose = new Pose();
		keyPoses[keyPoseIdx]->moveTowards(*keyPoses[keyPoseIdx+1], time, *currentTruthPose);
		truthPoses.push_back(currentTruthPose);

		Pose* estPose = new Pose();
		estPose->x = localizorSys->camPosition.at<float>(0);
		estPose->y = localizorSys->camPosition.at<float>(1);
		estPose->r = localizorSys->camRotation.at<float>(2);
		estPose->time = time;
		estimatedPoses.push_back(estPose);

		imshow("image", localizorSys->image);
		createVis(*estPose, *currentTruthPose);

		if(abs(rotDist(estPose->r, currentTruthPose->r)) >= 3) waitKey();

		char key = cv::waitKey(10);
		if(key != -1) handleKeys(key);

		time += 1./fps;
	}

	void finish() {
		float avgdist,avgrot,maxdist,maxrot;

		avgdist = 0;
		avgrot = 0;
		maxdist = 0;
		maxrot = 0;

		stringstream s;

		for(int i=0;i<estimatedPoses.size();i++) {
			float x = estimatedPoses[i]->x - truthPoses[i]->x;
			float y = estimatedPoses[i]->y - truthPoses[i]->y;
			//s << "x" << x << "=" << estimatedPoses[i]->x << "-" << truthPoses[i]->x << endl;
			//s << "y" << y << "=" << estimatedPoses[i]->y << "-" << truthPoses[i]->y << endl;
			s << "( "<<x<<", "<<y<<"): "<<sqrt(x*x+y*y)<<"         ";
			s << "( "<<estimatedPoses[i]->x<<", "<<estimatedPoses[i]->y<<")";
			s << "  -  ";
			s << "( "<<truthPoses[i]->x<<", "<<truthPoses[i]->y<<")";
			s << ": "<<sqrt(x*x+y*y)<<"   "<<endl;
			float curdist = sqrt(x*x+y*y);
			float currot = abs(rotDist(estimatedPoses[i]->r, truthPoses[i]->r));
			avgdist += curdist;
			avgrot += currot;
			if(curdist>maxdist) maxdist = curdist;
			if(currot>maxrot) maxrot = currot;

			//s << "avgdist+: " << sqrt(x*x+y*y) << endl;
		}

		avgdist /= estimatedPoses.size();
		avgrot /= estimatedPoses.size();

		s << endl;
		s << endl;

		for(int i=0;i<keyPoses.size();i++) {
			s << "Position: (" << keyPoses[i]->x << ", " << keyPoses[i]->y << ", " << "95" << "),";
			s << " Rotation: ( 0, 0, " << keyPoses[i]->r << ")" << endl;
		}

		s << endl;

		s << "avgdist:   " << avgdist << endl;
		s << "maxdist:   " << maxdist << endl;
		s << "avgrot:    " << avgrot << endl;
		s << "avgrotdeg: " << deg(avgrot) << endl;
		s << "maxrot:    " << maxrot << endl;
		s << "maxrotdeg: " << deg(maxrot) << endl;

		cout << s.str() << endl;

		ofstream myfile;
		myfile.open(outputfile);
		myfile << s.str();
		myfile.close();

		stringstream line;
		line << videofile << ","
				<< keyPoses[0]->x << ","
				<< keyPoses[0]->y << ","
				<< keyPoses[0]->r << ","
				<< (int)localizorSys->settings.multiMarkerStrategy << ","
				<< (int)localizorSys->settings.regulationStrategy << ","
				<< (int)localizorSys->settings.poseEstimationAlgorithm << ","
				<< avgdist << ","
				<< maxdist << ","
				<< avgrot << ","
				<< deg(avgrot) << ","
				<< maxrot << ","
				<< deg(maxrot) << endl;

		if(appendfile1 != "") {
			filereplacements(appendfile1);
			ofstream appfile1;
			appfile1.open(appendfile1, ios::out | ios::app);
			appfile1 << line.str();
			appfile1.close();
		}

		if(appendfile2 != "") {
			filereplacements(appendfile2);
			ofstream appfile2;
			appfile2.open(appendfile2, ios::out | ios::app);
			appfile2 << line;
			appfile2.close();
		}

		if(appendfile3 != "") {
			filereplacements(appendfile3);
			ofstream appfile3;
			appfile3.open(appendfile3, ios::out | ios::app);
			appfile3 << line.str();
			appfile3.close();
		}

	}

	void eventHandler(Listener::Event event) {
		switch(event) {
		case Listener::ITERATION:
			next();
			break;
		}
	}

	void createVis(Pose estPose, Pose truthPose) {
		vector<MarkerPose> poses = localizorSys->getPoses();
		// create 2d visualization
		Mat screen = Mat::zeros(480, 640, CV_8UC3);
		Mat orthoK = Mat::eye(3,3,CV_32FC1);
		fillMat3f(screen.cols/2., screen.rows/2., 1, orthoK, 2);
		Mat K;
		if(ortho) {
			K = orthoK;
		} else {
			K = localizorSys->getCamParams().CameraMatrix;
		}
		Scene vis2d = Scene(K, viscamt2d, viscamr2d, screen, ortho);
		vector<MarkerPose*> allposes;
		localizorSys->markerStorage.getAll(allposes);
		Mat t,r;
		for(int i=0;i<allposes.size();i++) {
			allposes[i]->get(t,r);
			vis2d.drawMarkerWire(allposes[i]->size, t, r, "", Scalar(128,128,128));
			vis2d.drawCameraWire(t, r, 5, Scalar(128,128,128));
		}
		for(int i=0;i<poses.size();i++) {
			poses[i].get(t,r);
			vis2d.drawMarkerWire(poses[i].size, t, r, "", Scalar(128,256,128));
			vis2d.drawCameraWire(t, r, 5, Scalar(128,256,128));
		}
		Mat estTvec, estRvec, truthTvec, truthRvec;
		estPose.getPosition(estTvec);
		estPose.getRotation(estRvec);
		truthPose.getPosition(truthTvec);
		truthPose.getRotation(truthRvec);
		vis2d.drawCameraWire(estTvec, estRvec, 8, Scalar(0,0,255));
		vis2d.drawCameraWire(truthTvec, truthRvec, 8, Scalar(0,255,255));
		Mat p;
		toMat3f(53, 468, 95, p);
		Mat zeros = Mat::zeros(3,1,CV_32FC1);
		vis2d.drawCameraWire(p, zeros, 8, Scalar(255,255,255));
		imshow("Visualization2d", *vis2d.getScreen());

		char key = cv::waitKey(10);
		if(key != -1) handleKeys(key);
	}


	void handleKeys(char key) {
		switch(key) {
		case 'q':
			viscamt2d.at<float>(0) -= 1;
			break;
		case 'w':
			viscamt2d.at<float>(0) += 1;
			break;

		case 'a':
			viscamt2d.at<float>(1) -= 1;
			break;
		case 's':
			viscamt2d.at<float>(1) += 1;
			break;

		case 'y':
			viscamt2d.at<float>(2) -= 1;
			break;
		case 'x':
			viscamt2d.at<float>(2) += 1;
			break;

		case 'u':
			viscamr2d.at<float>(0) -= 0.1;
			break;
		case 'i':
			viscamr2d.at<float>(0) += 0.1;
			break;

		case 'j':
			viscamr2d.at<float>(1) -= 0.1;
			break;
		case 'k':
			viscamr2d.at<float>(1) += 0.1;
			break;

		case 'n':
			viscamr2d.at<float>(2) -= 0.1;
			break;
		case 'm':
			viscamr2d.at<float>(2) += 0.1;
			break;

		case '#':
			ortho = !ortho;
			break;

		}
	}

};


#endif /* REALLIFETESTS_HPP_ */

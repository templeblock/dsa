/*
 * LocalizorSys.hpp
 *
 *  Created on: 11.02.2013
 *      Author: Bigontina
 */

#ifndef LOCALIZORSYS_HPP_
#define LOCALIZORSYS_HPP_


#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <math.h>

#include <utils.hpp>
#include <MarkerStorage.hpp>
#include <MarkerPose.hpp>
#include <Settings.hpp>
#include <Display.hpp>
#include <aruco/aruco.h>
#include <PoseEstimator.hpp>
#include <Scene.hpp>
#include <Listener.hpp>
#include <ExponentialSmoothing.hpp>
#include <RotationCalcs.hpp>
#include <LocalizorMulti.hpp>
#include <MarkerUtils.hpp>

using namespace std;

/*
 * This class starts the main loop and reads from the input stream
 * to estimate the pose for every frame.
 * It does so using the specified Multi-Marker-Strategy,
 * Rotation-Restriction, and Pose Estimation Algorithm.
 * It calls LocalizorMulti to delegate the task of pose estimation further.
 * It also retrieves the necessary underlying information
 * about global marker poses and settings from files.
 * It has no visual output.
 */
class LocalizorSys {
private:

	Display display;
	cv::VideoWriter videoWriter;
	bool writeVideo;
	Localizor localizor;
	map< Listener::Event, vector<Listener*> > eventListeners;
	vector<Marker> markers;
	vector<MarkerPose> poses;

public:
	Mat image;
	Mat camPosition;
	Mat camRotation;
	int markerCount;
	bool stopRequested;
	MarkerStorage markerStorage;
	Settings settings;

	LocalizorSys() {
		writeVideo = false;

		markerStorage.load();
		settings.load();
		stopRequested = false;
	}

	LocalizorSys(String config) {
		settings = Settings("config/"+config);
		settings.load();

		markerStorage = MarkerStorage("config/"+config+"/markers.yaml");
		markerStorage.load();

		camPosition = Mat::zeros(3,1,CV_32FC1);
		camRotation = Mat::zeros(3,1,CV_32FC1);

		stopRequested = false;
	}

	void addEventListener(Listener::Event event, Listener* listener) {
		std::map< Listener::Event, vector<Listener*> >::iterator it;
		it = eventListeners.find(event);
		if(it == eventListeners.end())
			eventListeners[event] = vector<Listener*>();
		eventListeners[event].push_back(listener);
	}

	void notifyListeners(Listener::Event event) {
		std::map< Listener::Event, vector<Listener*> >::iterator it;
		it = eventListeners.find(event);
		if(it == eventListeners.end()) return;
		vector<Listener*> listeners = it->second;
		for(int i=0;i<listeners.size();i++) {
			Listener* l = listeners[i];
			l->eventHandler(event);
		}
	}

	Mat* getImage() {
		return(&image);
	}

	Mat* getCamRotation() {
		return(&camRotation);
	}

	Mat* getCamPosition() {
		return(&camPosition);
	}

	CameraParameters getCamParams() {
		return settings.camParameters;
	}

	vector<Marker> getMarkers() {
		return markers;
	}

	vector<MarkerPose> getPoses() {
		return poses;
	}

	int getMarkerCount() {
		return(markerCount);
	}

	void run(cv::VideoCapture* capture) {
		cout << "LocalizorSys running..." << endl;
		capture->set(CV_CAP_PROP_FRAME_WIDTH, settings.camResoultion().width);
		capture->set(CV_CAP_PROP_FRAME_HEIGHT, settings.camResoultion().height);
		if(!capture->isOpened()) {
			std::cerr << " COULD NOT OPEN CAPTURE!" << endl;
			return;
		}

		aruco::CameraParameters camparams = settings.camParameters;

		Mat img;
		Mat displayimg;

		localizor = Localizor(settings.camParameters);
		Mat tvec, rvec, tvec_prev, rvec_prev;
		vector<aruco::Marker> markers;
		bool grabbed;
		Mat originalImg;
		ExponentialSmoothing smoothPos = ExponentialSmoothing(settings.exponentialSmoothingAlpha);
		ExponentialSmoothing smoothRot = ExponentialSmoothing(settings.exponentialSmoothingAlpha, true);
		while(true) {

			grabbed = capture->grab();
			if(grabbed) {
				cout << "frame grabbed" << endl;

				capture->retrieve(originalImg);
				undistort(originalImg, image, camparams.CameraMatrix, camparams.Distorsion);

				if(settings.flipVertical || settings.flipHorizontal) {
					int code = -1;
					if(!settings.flipVertical) code = 1;
					if(!settings.flipHorizontal) code = 0;
					flip(image, image, code);
				}

				if(settings.rotateImage) {
					rotateImage180(image);
				}

				if(writeVideo) {
					videoWriter.write(originalImg);
				}

				Mat newPosition, newRotation;
				camPosition.copyTo(newPosition);
				camRotation.copyTo(newRotation);

				markerCount = estimatePose(image, newPosition, newRotation);
				newRotation.at<float>(0,0) = 0; // remove x rotation
				newRotation.at<float>(0,1) = 0; // remove y rotation
				newPosition.at<float>(0,2) = 0; // remove z position

				smoothPos.add(newPosition);
				smoothRot.add(newRotation);
				smoothPos.get(camPosition);
				smoothRot.get(camRotation);

				notifyListeners(Listener::ITERATION);
			}

			if(stopRequested) break;
		}
	}

	void stop() {
		stopRequested = true;
	}

	void filterMarkersByPoses(vector<Marker> & markers, vector<MarkerPose> & poses) {
		for(int i=0;i<markers.size();i++) {
			if(markerStorage.contains(markers[i].id)) {
				markers[i].draw(image, Scalar(0,255,0), 2, true);
				poses.push_back(*markerStorage.getInfo(markers[i].id));
			} else {
				markers[i].draw(image, Scalar(0,0,255), 2, true);
				// The detected id is not used, it must have been an incorrect detection
				markers.erase(markers.begin()+i);
				i--;
			}
		}
	}

	int estimatePose(Mat & img, Mat & tvec, Mat & rvec) {
		vector<MarkerPose> poses;
		vector<Marker> markers;

		findMarkers(image, markers, settings.camParameters,
				settings.detectionThresholdBlockSize, settings.detectionThresholdSubtraction,
				settings.detectionMinMarker, settings.detectionMaxMarker);
		filterMarkersByPoses(markers, poses);

		this->markers = markers;
		this->poses = poses;

		if(markers.size() == 0) return(0);

//		cout << "settings.poseEstimationAlgorithm: " << settings.poseEstimationAlgorithm << endl;
//		cout << "settings.regulationStrategy: " << settings.regulationStrategy << endl;;
//		cout << "settings.multiMarkerStrategy: " << settings.multiMarkerStrategy << endl;;

		LocalizorMulti locmulti = LocalizorMulti(settings.camParameters,
				settings.poseEstimationAlgorithm,
				settings.regulationStrategy,
				settings.multiMarkerStrategy);

		vector<MarkerPose> correctedPoses;
		for(int i=0;i<poses.size();i++) {
			MarkerPose p = poses[i].copy();
			if(settings.invertYAxis) {
				p.position.at<float>(1) *= -1;
			}
			correctedPoses.push_back(p);
		}

		if(settings.invertYAxis & tvec.cols==1 & tvec.rows==3) {
			tvec.at<float>(1) *= -1;
		}

		locmulti.estimatePose(markers, correctedPoses, tvec, rvec);

		if(settings.invertYAxis) {
			tvec.at<float>(1) *= -1;
		}
	}
};


#endif /* LOCALIZORSYS_HPP_ */

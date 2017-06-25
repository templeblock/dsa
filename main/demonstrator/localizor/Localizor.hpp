/*
 * Localizor.hpp
 *
 *  Created on: Jan 8, 2013
 *      Author: Bigontina
 */

#ifndef LOCALIZOR_HPP_
#define LOCALIZOR_HPP_

#include <iostream>
#include <opencv2/opencv.hpp>

#include <utils.hpp>
#include <MarkerStorage.hpp>
#include <MarkerPose.hpp>
#include <Settings.hpp>
#include <Display.hpp>
#include <aruco.h>
#include <PoseEstimator.hpp>
#include <utils3d.hpp>
#include <MarkerUtils.hpp>
#include <PPP.hpp>
#include <RotationCalcs.hpp>
#include <RegulationStrategy.hpp>


using namespace std;


/*
 * This class calculates the pose for markers and
 * applies a rotation regulation strategy to the pose.
 * It delegates the task of pose estimation to PoseEstimator.
 * This is, except for the PARALLISM strategy; in this case
 * directly the PPP class is used to estimate the pose.
 */
class Localizor {
private:
	aruco::CameraParameters camParams;


public:
	PoseEstimationAlgorithm poseAlgorithm;
	RegulationStrategy regulationStrategy;

	Localizor() {}

	Localizor(aruco::CameraParameters camParams,
			PoseEstimationAlgorithm poseAlgo = PoseEstimationAlgorithm::ITERATIVE,
			RegulationStrategy regulationStrategy = RegulationStrategy::NONE) {
		this->camParams = camParams;
		setPoseAlgo(poseAlgo);
		this->regulationStrategy = regulationStrategy;
	}

	void setPoseAlgo(PoseEstimationAlgorithm algo) {
		this->poseAlgorithm = algo;
	}

	void estimatePose(Marker & marker, MarkerPose & pose, Mat & tvec, Mat & rvec, float rxweight=1, float ryweight=1) {
		vector<Marker> markers;
		vector<MarkerPose> poses;
		markers.push_back(marker);
		poses.push_back(pose);
		estimatePose(markers, poses, tvec, rvec, false, rxweight, ryweight);
	}

	void estimatePose(vector<Marker> & markers, vector<MarkerPose> & poses, Mat & tvec, Mat & rvec, bool usePoseGuess=false, float rxweight=1, float ryweight=1) {
		switch(this->regulationStrategy) {
			case RegulationStrategy::NONE:
				estimatePoseNormal(markers, poses, tvec, rvec);
				break;
			case RegulationStrategy::ROTATIONSUPPRESSION:
				estimatePoseRotationSuppression(markers, poses, tvec, rvec);
				break;
			case RegulationStrategy::REGULATIONPOINTS:
				estimatePoseRegulationPoints(markers, poses, tvec, rvec);
				break;
			case RegulationStrategy::PARALLELISM:
				estimatePoseParallel(markers, poses, tvec, rvec);
				break;
			default:
				estimatePoseNormal(markers, poses, tvec, rvec);
				break;
		}
	}


	void collectPoints(vector<Marker> & markers, vector<MarkerPose> & poses, Mat & imagePoints, Mat & objectPoints) {

		// collect image points
		imagePoints = Mat(2, markers.size()*4, CV_32FC1);
		for(int k=0;k<poses.size();k++) {
			for(int i=0;i<markers.size();i++) {
				if(markers[i].id == poses[k].id) {
					for(int j=0;j<4;j++) {
						imagePoints.at<float>(0,k*4+j) = markers[i][j].x;
						imagePoints.at<float>(1,k*4+j) = camParams.CamSize.height- markers[i][j].y;
					}
				}
			}
		}

		// collect object points
		Mat tmp;
		Mat markerObjectPoints3h;
		Mat T, p, r;
		objectPoints = Mat(3, poses.size()*4, CV_32FC1);
		for(int i=0;i<poses.size();i++) {
			genMarkerPoints(poses[i].size, markerObjectPoints3h, true);
			poses[i].get(p,r);
			generateTranformationMatrix(r, p, T);
			Mat x = T * markerObjectPoints3h;
			tmp = objectPoints.colRange(i*4,i*4+4);
			normHomogeneous2(x, tmp, true, false);
		}

	}

	void estimateObjectPose(vector<Marker> & markers, vector<MarkerPose> & poses, Mat & tvec, Mat & rvec, bool usePoseGuess=false, float rxweight=1, float ryweight=1) {
		Mat imagePoints, objectPoints, tmp;

		collectPoints(markers, poses, imagePoints, objectPoints);

		PoseEstimator e = PoseEstimator(camParams.CameraMatrix, poseAlgorithm);
		e.getPose(objectPoints, imagePoints, tvec, rvec);
	}

	void objectPoseToCamPose(Mat & tvec, Mat & rvec) {
		tvec = tvec*(-1);
		rvec *= -1;
		Mat R;
		Rodrigues(rvec, R);
		tvec = R*tvec;
	}

	void estimatePoseNormal(vector<Marker> & markers, vector<MarkerPose> & poses, Mat & tvec, Mat & rvec, bool usePoseGuess=false, float rxweight=1, float ryweight=1) {
		Mat imagePoints, objectPoints, tmp;

		collectPoints(markers, poses, imagePoints, objectPoints);

		PoseEstimator e = PoseEstimator(camParams.CameraMatrix, this->poseAlgorithm);
		e.getPose(objectPoints, imagePoints, tvec, rvec, usePoseGuess);

		objectPoseToCamPose(tvec, rvec);
	}

	// Using this method did not led to any useful results
	void estimatePoseRegulationPoints(vector<Marker> & markers, vector<MarkerPose> & poses, Mat & tvec, Mat & rvec) {
		Mat imagePoints, objectPoints, imagePointsExt, objectPointsExt;

		collectPoints(markers, poses, imagePoints, objectPoints);

		imagePointsExt = Mat(2, imagePoints.cols+1, imagePoints.type());
		objectPointsExt = Mat(3, objectPoints.cols+1, objectPoints.type());

		imagePoints.copyTo(imagePointsExt.colRange(0,imagePoints.cols));
		objectPoints.copyTo(objectPointsExt.colRange(0,objectPoints.cols));

		float cx,cy;
		cx = camParams.CameraMatrix.at<float>(0,2);
		cy = camParams.CameraMatrix.at<float>(1,2);
		fillMat2f(cx, cy, imagePointsExt, imagePoints.cols);
		fillMat3f(1100, 1100, 1000000, objectPointsExt, objectPoints.cols);

		Mat regp2 = imagePointsExt.col(imagePointsExt.cols-1);
		Mat regp3 = objectPointsExt.col(objectPointsExt.cols-1);

		Mat P;
		Mat rcam = Mat::zeros(3,1,CV_32FC1);
		Mat tcam = Mat::zeros(3,1,CV_32FC1);
		generateProjectionMatrix(rcam, tcam, camParams.CameraMatrix, P, true);

		Mat regp3proj;
		project(P, regp3, regp3proj);

		PoseEstimator e = PoseEstimator(camParams.CameraMatrix, poseAlgorithm);
		e.getPose(objectPointsExt, imagePointsExt, tvec, rvec);

		objectPoseToCamPose(tvec, rvec);
	}

	void estimatePoseRotationSuppression(vector<Marker> & markers, vector<MarkerPose> & poses, Mat & tvec, Mat & rvec) {
		Mat imagePoints, objectPoints, tmp;

		collectPoints(markers, poses, imagePoints, objectPoints);

		// Center points (so pose estimation algorithm finds rotations around center)
		Mat center;
		avg(objectPoints, center);

		// TODO: Do that directly:
		Mat zeros = Mat::zeros(3,1,CV_32FC1);
		Mat trans = center*(-1);
		Mat T;
		generateTranformationMatrix(zeros, trans, T);
		transform3d(T, objectPoints, objectPoints);

		PoseEstimator e = PoseEstimator(camParams.CameraMatrix, this->poseAlgorithm);
		e.getPose(objectPoints, imagePoints, tvec, rvec);

		// This could be adjusted to allow for a small percentage of rotation
		float rxweight = 0;
		float ryweight = 0;

		rvec.at<float>(0) *= rxweight;
		rvec.at<float>(1) *= ryweight;

		objectPoseToCamPose(tvec, rvec);

		// move back to old position
		tvec += center;
	}

	void estimatePoseParallel(vector<Marker> & markers, vector<MarkerPose> & poses, Mat & tvec, Mat & rvec) {
		if(markers.size() != 1) {
			cerr << "EXCEPTION: Pose Parallel Algorithm only works with 1 marker" << endl;
			cerr << "    in: estimatePoseParallel" << endl;
			throw;
		}

		// collect image points
		Mat imagePoints;
		imagePoints = Mat(2, markers.size()*4, CV_32FC1);
		for(int j=0;j<4;j++) {
			imagePoints.at<float>(0,j) = markers[0][j].x;
			imagePoints.at<float>(1,j) = camParams.CamSize.height- markers[0][j].y;
		}

		PPP ppp = PPP();
		ppp.getPose(imagePoints, poses[0].size, camParams.CameraMatrix, tvec, rvec);

		// move marker to origin;
		objectPoseToCamPose(tvec, rvec);

		// move marker to marker pose
		Mat T;
		generateTranformationMatrix(poses[0].rotation, poses[0].position, T);
		transform3d(T, tvec, tvec);
		rvec += poses[0].rotation;
		normalizeRotation(rvec);
	}

};



#endif /* LOCALIZOR_HPP_ */

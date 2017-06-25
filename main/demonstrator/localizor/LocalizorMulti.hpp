/*
 * LocalizorMulti.hpp
 *
 *  Created on: 21.03.2013
 *      Author: Bigontina
 */

#ifndef LOCALIZORMULTI_HPP_
#define LOCALIZORMULTI_HPP_

#include <Localizor.hpp>
#include <RotationCalcs.hpp>
#include <MultiMarkerStrategy.hpp>
#include <PoseEstimationAlgorithm.hpp>
#include <RegulationStrategy.hpp>

/*
 * Estimates the poses of a set of markers and combines
 * them to a final pose. The actual pose estimation is
 * delegated further to the Localizor class.
 */
class LocalizorMulti {
private:
	MultiMarkerStrategy multiMarkerStrategy;
	RegulationStrategy regulationStrategy;
	PoseEstimationAlgorithm poseEstimationAlgorithm;
	CameraParameters camparams;

public:

	LocalizorMulti() {}
	LocalizorMulti(CameraParameters camparams,
			PoseEstimationAlgorithm poseEstimationAlgorithm = PoseEstimationAlgorithm::ITERATIVE,
			RegulationStrategy regulatioStrategy = RegulationStrategy::NONE,
			MultiMarkerStrategy multiMarkerStrategy = MultiMarkerStrategy::AVERAGE) {
		this->camparams = camparams;
		this->multiMarkerStrategy = multiMarkerStrategy;
		this->regulationStrategy = regulatioStrategy;
		this->poseEstimationAlgorithm = poseEstimationAlgorithm;
	}

	void estimatePose(vector<Marker> markers, vector<MarkerPose> poses, Mat & tvec, Mat & rvec) {
		switch(this->multiMarkerStrategy) {
			case MultiMarkerStrategy::AVERAGE:
				estimatePoseAvg(markers, poses, tvec, rvec);
				break;
			case MultiMarkerStrategy::WGTAVERAGE:
				estimatePoseWgtAvg(markers, poses, tvec, rvec);
				break;
			case MultiMarkerStrategy::WGTAVERAGEPREV:
				estimatePoseWgtAvgPrev(markers, poses, tvec, rvec);
				break;
			case MultiMarkerStrategy::GLOBALOPT:
				estimatePoseGlobalOpt(markers, poses, tvec, rvec);
				break;
			default:
				estimatePoseAvg(markers, poses, tvec, rvec);
				break;
		}
	}

	int estimatePoseGlobalOpt(vector<Marker> markers, vector<MarkerPose> poses, Mat & tvec, Mat & rvec) {
		Localizor localizor = Localizor(camparams, poseEstimationAlgorithm, regulationStrategy);
		localizor.estimatePose(markers, poses, tvec, rvec);
	}

	int estimatePoseAvg(vector<Marker> markers, vector<MarkerPose> poses, Mat & tvec, Mat & rvec) {
		vector<Mat> tvecs;
		vector<Mat> rvecs;

		Localizor localizor = Localizor(camparams, poseEstimationAlgorithm, regulationStrategy);

		// estimate the camera poses one by one
		for(int i=0;i<markers.size(); i++) {
			Mat tvec, rvec;
			localizor.estimatePose(markers[i], poses[i], tvec, rvec);
			tvecs.push_back(tvec);
			rvecs.push_back(rvec);
		}

		// calculate averages
		rearrangeRotations(rvecs);
		Mat tvecsum = Mat::zeros(3,1,CV_32FC1);
		Mat rvecsum = Mat::zeros(3,1,CV_32FC1);
		for(int i=0;i<tvecs.size();i++) {
			tvecsum += tvecs[i];
			rvecsum += rvecs[i];
		}
		Mat tvecavg = tvecsum/tvecs.size();
		Mat rvecavg = rvecsum/rvecs.size();
		normalizeRotation(rvecavg);

		tvec = tvecavg;
		rvec = rvecavg;
	}

	int estimatePoseWgtAvg(vector<Marker> markers, vector<MarkerPose> poses, Mat & tvec, Mat & rvec) {
		vector<Mat> tvecs;
		vector<Mat> rvecs;

		// controlDist and controlWeight can be used to tune the weighting
		// A distance (controlDist) can be chosen at which the pose will be weighted with a specified weight (controlWeight)
		// interpolation is linearly with weight 1 at center.
		float controlDist = sqrt(pow(camparams.CamSize.height/2.,2) + pow(camparams.CamSize.width/2.,2));
		float controlWeight = 0.2;

		float maxdist = controlDist * 1/(1-controlWeight);

		Localizor localizor = Localizor(camparams, poseEstimationAlgorithm, regulationStrategy);

		// estimate the camera poses one by one
		for(int i=0;i<markers.size(); i++) {
			Mat t, r;
			localizor.estimatePose(markers[i], poses[i], t, r);
			tvecs.push_back(t);
			rvecs.push_back(r);
		}

		if(markers.size() == 1) {
			tvec = tvecs[0];
			rvec = rvecs[0];
			return(1);
		}

		rearrangeRotations(rvecs);

		float cx = camparams.CameraMatrix.at<float>(0,2);
		float cy = camparams.CameraMatrix.at<float>(1,2);

		float weightsum = 0;
		vector<float> weights;
		for(int i=0;i<markers.size();i++) {
			Point2f mc = markers[i].getCenter();
			float dist = sqrt(pow(mc.x - cx, 2) + pow(mc.y - cy, 2));
			float weight = 1-dist/maxdist;
			if(weight < 0) weight = 0;

			weights.push_back(weight);
			weightsum += weight;
		}

		// prevent division by zero (should actually never be the case anyway)
		if(weightsum == 0) {
			tvec = tvecs[0];
			rvec = rvecs[0];
		} else {
			Mat newtvec = Mat::zeros(3,1,CV_32FC1);
			Mat newrvec = Mat::zeros(3,1,CV_32FC1);
			float percentage;
			for(int i=0;i<tvecs.size();i++) {
				percentage = (weights[i]/weightsum);
				newtvec += tvecs[i] * percentage;
				newrvec += rvecs[i] * percentage;
			}
			tvec = newtvec;
			rvec = newrvec;
		}

		normalizeRotation(rvec);
	}

	int estimatePoseWgtAvgPrev(vector<Marker> markers, vector<MarkerPose> poses, Mat & tvec, Mat & rvec) {
		vector<Mat> tvecs;
		vector<Mat> rvecs;

		// TODO: put that into the settings file
		float maxDistance = pow(37*2,2); // 37 is the max distance for 15 fps, and 20km/h; double it for some play

		Localizor localizor = Localizor(camparams, poseEstimationAlgorithm, regulationStrategy);

		// estimate the camera poses one by one
		for(int i=0;i<markers.size(); i++) {
			Mat t, r;
			localizor.estimatePose(markers[i], poses[i], t, r);
			tvecs.push_back(t);
			rvecs.push_back(r);
		}

		if(markers.size() == 1) {
			tvec = tvecs[0];
			rvec = rvecs[0];
			return(1);
		}

		rearrangeRotations(rvecs);

		float sqdistsum = 0;
		vector<float> sqdists;
		for(int i=0;i<tvecs.size();i++) {
			Mat diff = tvecs[i] - tvec;
			float t0 = diff.at<float>(0);
			float t1 = diff.at<float>(1);
			float t2 = diff.at<float>(2);
			float sqdist = t0*t0 + t1*t1 + t2*t2;
			float weight = maxDistance - sqdist;
			if(weight < 0) {
				weight = 0;
			}

			sqdistsum += weight;
			sqdists.push_back(weight);
		}

		if(sqdistsum == 0) {
			// if no valid sum exists, weight every pose equally
			for(int i=0;i<sqdists.size();i++) {
				sqdists[i] = 1;
			}
			sqdistsum = sqdists.size();
		}

		Mat newtvec = Mat::zeros(3,1,CV_32FC1);
		Mat newrvec = Mat::zeros(3,1,CV_32FC1);
		for(int i=0;i<tvecs.size();i++) {
			float weight = (1-sqdists[i]/sqdistsum);
			newtvec += tvecs[i] * weight;
			newrvec += rvecs[i] * weight;
		}
		tvec = newtvec;
		rvec = newrvec;

		normalizeRotation(rvec);
	}

};


#endif /* LOCALIZORMULTI_HPP_ */

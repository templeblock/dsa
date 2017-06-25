/*
 * SyntheticTests.hpp
 *
 *  Created on: 10.02.2013
 *      Author: Bigontina
 */

#ifndef SYNTHETICTESTS_HPP_
#define SYNTHETICTESTS_HPP_

#include <opencv2/opencv.hpp>
#include <aruco/cameraparameters.h>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <math.h>

#include <ViewSimulator.hpp>
#include <MarkerGeneration.hpp>
#include <Rnd.hpp>
#include <LocalizorMulti.hpp>
#include <MarkerUtils.hpp>

using namespace cv;
using namespace std;
using namespace aruco;


/*
 * Able to perform various synthetic tests.
 * This class is not used by the actual pose estimation system.
 */

class SyntheticTests {

public:
	void singlePoseTest(float markerSize, cv::Mat tvec, cv::Mat rvec, CameraParameters camparams, PoseEstimationAlgorithm poseAlgo,
			Mat & terr, Mat & rerr, bool & found, int error=0)
	{
		Mat tvec_est, rvec_est;
		Mat modelPoints, projectedPoints;
		ViewSimulator sim = ViewSimulator();

		genMarkerPoints(markerSize, modelPoints, false);

		Mat K, F, P;
		K = camparams.CameraMatrix;
		generateFlipMatrix(camparams.CamSize.height, F);
		generateProjectionMatrix(rvec, tvec, F*K, P, true);

		project(P, modelPoints, projectedPoints);

		Rnd rnd = Rnd();

		float r;
		for(int i=0;i<projectedPoints.cols;i++) {
			projectedPoints.at<float>(0,i) += rnd.next(-error, error);
			projectedPoints.at<float>(1,i) += rnd.next(-error, error);
		}

		vector<Point2f> pointsVec;
		mat2fToPoints(projectedPoints, pointsVec);
		Marker marker = Marker(pointsVec, 1);

		Mat pos = Mat::zeros(3,1,CV_32FC1);
		Mat rot = Mat::zeros(3,1,CV_32FC1);
		MarkerPose pose = MarkerPose(1, pos, rot, markerSize);

		Localizor localizor = Localizor(camparams);
		localizor.setPoseAlgo(poseAlgo);
		localizor.estimatePose(marker, pose, tvec_est, rvec_est);

		rotationDist(rvec, rvec_est, rerr);
		terr = tvec_est - tvec;

		found = true;
	}

	void performPoseSeries(PoseEstimationAlgorithm poseAlgo, int zrotation=0, float detectionerror=0.0)
	{
		float r = (zrotation/180.*M_PI);
		Size imageResolution(1280, 960);
		float markerSize = 50;

		Mat K;
		generateIntrinsicsMatrix(900, 900, 384, 288, K);
		Mat distortion = Mat::zeros(4,1,CV_32FC1);
		Size camSize = Size(768, 576);
		CameraParameters camParams = CameraParameters(K, distortion, camSize);

		Mat markerimg, tvec, rvec, tvec_est, rvec_est, dstimg;
		toMat3f(0, 0, 100, tvec);
		toMat3f(0, r, 0, rvec);
		Mat terr, rerr;
		bool found;

		int repetitions = detectionerror==0?1:10;

		char k;
		ostringstream res;
		bool wait=true;
		float terrsum, rerrsum;
		for(float d=-200;d>-2000;d-=10){//d+=0.1) {
			fillMat3f(0, 0, d, tvec);
			terrsum = rerrsum = 0;
			for(int j=0;j<repetitions;j++) {
				singlePoseTest(markerSize, tvec, rvec, camParams, poseAlgo, terr, rerr, found, detectionerror);
				terrsum += dist(terr);
				//				rerrsum += dist(rerr);
			}
			terr = terrsum/(float)repetitions;
			//			rerr = rerrsum/(float)repetitions;
			res << "(" << (-d/100.0) << "," << (dist(terr)) << ")" << endl;
			cout << "(" << (-d/100.0) << "," << (dist(terr)) << ")" << endl;
		}

		ofstream myfile;
		ostringstream filename;
		filename << "detection_d_terr_algo" << poseAlgo << "_" << zrotation << ".txt";
		string filename2 = filename.str();
		myfile.open(filename2);
		myfile << res.str();
		myfile.close();
	}

	void genMarkerPoses(int count, float markerSize, vector<MarkerPose> & poses) {
		for(int i=0;i<count;i++) {
			MarkerPose p = MarkerPose(i, poses[i].position, poses[i].rotation, markerSize);
			poses.push_back(p);
		}
	}

	bool singleTest(float markerSize, cv::Mat tvec, cv::Mat rvec, CameraParameters camparams, PoseEstimationAlgorithm poseAlgo,
			Mat & terr, Mat & rerr, bool & found)
	{
		Mat markerimg, tvec_est, rvec_est, dstimg;
		markerimg = createMarker(11);

		Mat rcam, tcam;
		toMat3f(0.,0.,0.,rcam);
		toMat3f(0.,0.,0.,tcam);

		ViewSimulator* sim = new ViewSimulator();
		sim->generateView(markerimg, markerSize*9./7., rcam, tcam, rvec, tvec, camparams.CameraMatrix, camparams.CamSize, dstimg);

		cv::namedWindow("sim", CV_WINDOW_AUTOSIZE);
		cv::imshow("sim", dstimg);

		Localizor* l = new Localizor(camparams);
		l->setPoseAlgo(poseAlgo);
		//found = l->estimatePose(dstimg, tvec_est, rvec_est);

		vector<Marker> markers;
		vector<MarkerPose> poses;

		findMarkers(dstimg, markers, camparams);
		genMarkerPoses(markers.size(), markerSize, poses);
		l->estimatePose(markers, poses, tvec_est, rvec_est);

	}

	void performSeries() {
		Size imageResolution(1280, 960);
		float markerSize = 50;
		Settings* s = new Settings();
		s->load(imageResolution);
		Mat markerimg, tvec, rvec, tvec_est, rvec_est, dstimg;
		toMat3f(0, 0, 100, tvec);
		toMat3f(0, 0, 0, rvec);
		Mat terr, rerr;
		bool found;

		bool wait = true;
		float maxDist = 0;
		ostringstream res;
		for(float d=500;d<4000;d+=50){//d+=0.1) {
			float r;
			for(r=0;r<1.5;r+=0.1){
				fillMat3f(0, 0, d, tvec);
				fillMat3f(0, r, 0, rvec);
				singleTest(markerSize, tvec, rvec, s->camParameters, PoseEstimationAlgorithm::ITERATIVE, terr, rerr, found);

				if(wait) {
					char k = cv::waitKey(0);
					if(k == 'q'){
						r = 1000;
						break;
					}
					if(k == 's')
						wait = false;
				} else {
					cv::waitKey(10);
				}
				if(!found) break;
			}
			res << "(" << (d/100.0) << "," << (r/M_PI*180.0) << ")" << endl;
		}
		cout << res.str();
		ofstream myfile;
		myfile.open ("detection_d_r.txt");
		myfile << res.str();
		myfile.close();
	}

	void projectMarker(Mat & P, MarkerPose & pose, Marker & m, float detectionError = 0.0) {
		Mat markerPoints;
		genMarkerPoints(pose.size, markerPoints,false);

		Mat T;
		generateTranformationMatrix(pose.rotation, pose.position, T);

		Mat imagePoints;
		project(P*T, markerPoints, imagePoints);

		if(detectionError > 0) {
			Rnd rnd = Rnd();
			for(int i=0;i<imagePoints.cols;i++) {
				imagePoints.at<float>(0,i) += rnd.next(-detectionError, detectionError);
				imagePoints.at<float>(1,i) += rnd.next(-detectionError, detectionError);
			}
		}

		vector<Point2f> pointsVec;
		mat2fToPoints(imagePoints, pointsVec);
		m = Marker(pointsVec, pose.id);
	}

	void singleTestMultiMarker(float markerSize, CameraParameters camparams, Mat rcam, Mat tcam,
			PoseEstimationAlgorithm poseAlgo, RegulationStrategy regulationStrategy, MultiMarkerStrategy multiMarkerStrategy,
			Mat & terr, Mat & rerr, float detectionError=0, float poseError=0,
			float randomCamRotation=0, float randomCamMovement=0, int markersDetected=3) {

		Mat pos1, rot1;
		toMat3f(-50,-50,400,pos1);
		toMat3f(0,0,0,rot1);
		MarkerPose pose1 = MarkerPose(1, pos1, rot1, markerSize);

		Mat pos2, rot2;
		toMat3f( 50,-50,400,pos2);
		toMat3f(0,0,0,rot2);
		MarkerPose pose2 = MarkerPose(2, pos2, rot2, markerSize);

		Mat pos3, rot3;
		toMat3f(0,50,400,pos3);
		toMat3f(0,0,0,rot3);
		MarkerPose pose3 = MarkerPose(3, pos3, rot3, markerSize);

		Rnd rnd = Rnd();

		vector<MarkerPose> poses;
		if(poses.size()+2 < markersDetected || rnd.nextBool()) {
			poses.push_back(pose1);
		}
		if(poses.size()+1 < markersDetected || (poses.size() < markersDetected && rnd.nextBool())) {
			poses.push_back(pose2);
		}
		if(poses.size() < markersDetected) {
			poses.push_back(pose3);
		}

		if(poses.size() != markersDetected) {
			cerr << "Choosing markers randomly did not work! Quitting." << endl;
			cerr << "(markersDetected = " << markersDetected << ")" << endl;
			cerr << "(poses.size() = " << poses.size() << ")" << endl;
			throw;
		}


		// move poses to cam x and y position, so cam is placed benieve markers
		for(int i=0;i<poses.size();i++) {
			poses[i].position.at<float>(0) += tcam.at<float>(0);
			poses[i].position.at<float>(1) += tcam.at<float>(1);
		}

		// add an error to marker pose (simulate inaccurately placed markers)
		vector<MarkerPose> poseserr;
		for(int i=0;i<poses.size();i++) {
			poseserr.push_back(poses[i].copy());
			poseserr[i].position.at<float>(0) += rnd.next(-poseError, poseError);
			poseserr[i].position.at<float>(1) += rnd.next(-poseError, poseError);
		}

		// add random cam rotation (simulate shaking camera)
		Mat rcamerr = Mat(3,1,CV_32FC1);
		rcam.copyTo(rcamerr);
		rcamerr.at<float>(0) += rnd.next(-randomCamRotation, randomCamRotation);
		rcamerr.at<float>(1) += rnd.next(-randomCamRotation, randomCamRotation);

		// add random cam movement
		Mat tcamerr = Mat(3,1,CV_32FC1);
		tcam.copyTo(tcamerr);
		tcamerr.at<float>(0) += rnd.next(-randomCamMovement, randomCamMovement);
		tcamerr.at<float>(1) += rnd.next(-randomCamMovement, randomCamMovement);

		// create projection matrix
		Mat K, F, P;
		K = camparams.CameraMatrix;
		generateFlipMatrix(camparams.CamSize.height, F);
		generateProjectionMatrix(rcamerr, tcamerr, F*K, P, true);

		// create markers from pose (by projection)
		vector<Marker> markers;
		for(int i=0;i<poseserr.size();i++) {
			Marker m;
			projectMarker(P, poseserr[i], m, detectionError);
			markers.push_back(m);
		}

		// print setup:
		cout << "Camera: " << endl;
		cout << "  " << "Rot: " << rcamerr << endl;
		cout << "  " << "Pos: " << tcamerr << endl;
		for(int i=0;i<markers.size();i++) {
			cout << "Marker " << markers[i].id << ":" << endl;
			cout << "  " << "Pose: " << endl;
			cout << "    " << "Rot: " << poses[i].rotation << " (" << poseserr[i].rotation << ")" << endl;
			cout << "    " << "Pos: " << poses[i].position << " (" << poseserr[i].position << ")" << endl;
			cout << "  " << "Points: " << endl;
			cout << "    " << markers[i][0] << endl;
			cout << "    " << markers[i][1] << endl;
			cout << "    " << markers[i][2] << endl;
			cout << "    " << markers[i][3] << endl;
		}


		// perform localization:
		LocalizorMulti locMulti = LocalizorMulti(camparams, poseAlgo, regulationStrategy, multiMarkerStrategy);
		Mat tvec, rvec;
		locMulti.estimatePose(markers, poses, tvec, rvec);

		rotationDist(rcamerr, rvec, rerr);
		terr = tvec - tcamerr;
	}

	void performSeriesMultiMarker(String configfile) {

		vector<MultiMarkerStrategy> mms;
		mms.push_back(MultiMarkerStrategy::AVERAGE);
		mms.push_back(MultiMarkerStrategy::WGTAVERAGE);
		//mms.push_back(MultiMarkerStrategy::WGTAVERAGEPREV);
		mms.push_back(MultiMarkerStrategy::GLOBALOPT);

		map<MultiMarkerStrategy,String> mmsnames;
		mmsnames.insert(make_pair(MultiMarkerStrategy::AVERAGE, "Average"));
		mmsnames.insert(make_pair(MultiMarkerStrategy::WGTAVERAGE, "Weighted Average"));
		mmsnames.insert(make_pair(MultiMarkerStrategy::GLOBALOPT, "Global Optimum"));

		vector<RegulationStrategy> res;
		res.push_back(RegulationStrategy::NONE);
		res.push_back(RegulationStrategy::ROTATIONSUPPRESSION);
		res.push_back(RegulationStrategy::PARALLELISM);

		map<RegulationStrategy,String> resnames;
		resnames.insert(make_pair(RegulationStrategy::NONE, "None"));
		resnames.insert(make_pair(RegulationStrategy::ROTATIONSUPPRESSION, "Rotation Suppression"));
		resnames.insert(make_pair(RegulationStrategy::PARALLELISM, "Parallelism Algorithm"));

		vector<PoseEstimationAlgorithm> pea;
		pea.push_back(PoseEstimationAlgorithm::ITERATIVE);
		pea.push_back(PoseEstimationAlgorithm::RPPAlgo);
		pea.push_back(PoseEstimationAlgorithm::PLANARPOSIT);

		map<PoseEstimationAlgorithm,String> peanames;
		peanames.insert(make_pair(PoseEstimationAlgorithm::ITERATIVE, "Iterative"));
		peanames.insert(make_pair(PoseEstimationAlgorithm::RPPAlgo, "RPP"));
		peanames.insert(make_pair(PoseEstimationAlgorithm::PLANARPOSIT, "Planar POSIT"));


		FileStorage fs(configfile, FileStorage::READ);
		float detectionError = (float)fs["detectionError"];
		float poseError = (float)fs["poseError"];
		float camRotation = rad((float)fs["camRotationError"]);

		float camMovement = (float)fs["camMovement"];
		int detectedMarkers = (int)fs["detectedMarkers"];;

		float markerSize = (float)fs["markerSize"];
		int repetitions = (int)fs["repetitions"];

		String outputfile = (String)fs["outputfile"];
		fs.release();


		vector<Mat> terrs;
		vector<Mat> rerrs;
		vector<Mat> tmaxs;
		vector<Mat> rmaxs;
		vector<Mat> txymaxs;
		vector<float> rzmaxs;
		for(int i=0;i<mms.size();i++) {
			for(int j=0;j<res.size();j++) {
				for(int k=0;k<pea.size();k++) {
					if(mms[i] == MultiMarkerStrategy::GLOBALOPT && res[j] == RegulationStrategy::PARALLELISM) {
						continue;
					}
					Mat terr, rerr, tmax, rmax, txymax;
					float rzmax;
					performSeriesMultiMarkerAvg(markerSize, repetitions, mms[i], res[j], pea[k],
							detectionError, poseError, camRotation, camMovement, detectedMarkers,
							terr, rerr, tmax, rmax, txymax, rzmax);
					terrs.push_back(terr);
					rerrs.push_back(rerr);
					tmaxs.push_back(tmax);
					rmaxs.push_back(rmax);
					txymaxs.push_back(txymax);
					rzmaxs.push_back(rzmax);
					if(res[j] == RegulationStrategy::PARALLELISM) {
						// dont iterate every pea!, they are not used in this case anyway
						break;
					}
				}
			}
		}


		ostringstream out;

		out << "mms,";
		out << "res,";
		out << "pea,";
		out << "terrx,";
		out << "terry,";
		out << "terrz,";
		out << "terrxydist,";
		out << "terrdist,";
		out << "tmaxdistx,";
		out << "tmaxdisty,";
		out << "tmaxdistz,";
		out << "tmaxdist,";
		out << "tmaxxydistx,";
		out << "tmaxxydisty,";
		out << "tmaxxydist,";
		out << "rerrx,";
		out << "rerrxdeg,";
		out << "rerry,";
		out << "rerrydeg,";
		out << "rerrz,";
		out << "rerrzdeg,";
		out << "rerrdist,";
		out << "rerrdistdeg,";
		out << "rmaxdistx,";
		out << "rmaxdistxdeg,";
		out << "rmaxdisty,";
		out << "rmaxdistydeg,";
		out << "rmaxdistz,";
		out << "rmaxdistzdeg,";
		out << "rmaxdist,";
		out << "rmaxdistdeg,";
		out << "rmaxz" << ",";
		out << "rmaxzdeg" << endl;
		int c = 0;
		for(int i=0;i<mms.size();i++) {
			for(int j=0;j<res.size();j++) {
				for(int k=0;k<res.size();k++) {
					if(mms[i] == MultiMarkerStrategy::GLOBALOPT && res[j] == RegulationStrategy::PARALLELISM) {
						continue;
					}
					out << mmsnames[mms[i]] << ",";
					out << resnames[res[j]] << ",";
					if(res[j] == RegulationStrategy::PARALLELISM) {
						out << "-" << ",";
					} else {
						out << peanames[pea[k]] << ",";
					}
					if(terrs[c].cols!=1) {
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
					} else {
						out << terrs[c].at<float>(0) << ",";
						out << terrs[c].at<float>(1) << ",";
						out << terrs[c].at<float>(2) << ",";
						out << dist2f(terrs[c].rowRange(0,2)) << ",";
						out << dist(terrs[c]) << ",";
					}
					if(tmaxs[c].cols!=1) {
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
					} else {
						out << tmaxs[c].at<float>(0) << ",";
						out << tmaxs[c].at<float>(1) << ",";
						out << tmaxs[c].at<float>(2) << ",";
						out << dist(tmaxs[c]) << ",";
					}
					if(txymaxs[c].cols!=1) {
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
					} else {
						out << txymaxs[c].at<float>(0) << ",";
						out << txymaxs[c].at<float>(1) << ",";
						out << dist(txymaxs[c]) << ",";
					}
					if(rerrs[c].cols!=1) {
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
					} else {
						out << rerrs[c].at<float>(0) << ",";
						out << deg(rerrs[c].at<float>(0)) << ",";
						out << rerrs[c].at<float>(1) << ",";
						out << deg(rerrs[c].at<float>(1)) << ",";
						out << rerrs[c].at<float>(2) << ",";
						out << deg(rerrs[c].at<float>(2)) << ",";
						out << dist(rerrs[c]) << ",";
						out << deg(dist(rerrs[c])) << ",";
					}
					if(rmaxs[c].cols!=1) {
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
						out << -1 << ",";
					} else {
						out << rmaxs[c].at<float>(0) << ",";
						out << deg(rmaxs[c].at<float>(0)) << ",";
						out << rmaxs[c].at<float>(1) << ",";
						out << deg(rmaxs[c].at<float>(1)) << ",";
						out << rmaxs[c].at<float>(2) << ",";
						out << deg(rmaxs[c].at<float>(2)) << ",";
						out << dist(rmaxs[c]) << ",";
						out << deg(dist(rmaxs[c])) << ",";
					}
					out << rzmaxs[c] << ",";
					out << deg(rzmaxs[c]) << endl;
					c++;
					if(res[j] == RegulationStrategy::PARALLELISM) {
						// dont iterate every pea!, they are not used in this case anyway
						break;
					}
				}
			}
		}

		cout << endl << endl << endl;
		cout << out.str();

		ofstream myfile;
		myfile.open (outputfile);
		myfile << out.str();
		myfile.close();


	}

	void performSeriesMultiMarkerAvg(
			float markerSize,
			int repetitions,
			MultiMarkerStrategy multiMarkerStrategy,
			RegulationStrategy regulationStrategy,
			PoseEstimationAlgorithm poseAlgo,
			float detectionError,
			float poseError,
			float camRotation,
			float camMovement,
			int detectedMarkers,
			Mat & terr,
			Mat & rerr,
			Mat & maxterr,
			Mat & maxrerr,
			Mat & maxtxyerr,
			float & maxrzerr) {


		// set a random z-rotation
		Rnd rnd = Rnd();
		float zrotation = rnd.next(-M_PI, M_PI);

		Mat K;
		generateIntrinsicsMatrix(900, 900, 384, 288, K);
		Mat distortion = Mat::zeros(4,1,CV_32FC1);
		Size camSize = Size(768, 576);
		CameraParameters camParams = CameraParameters(K, distortion, camSize);

		Mat tcam, rcam;
		toMat3f(1000, 1000, 100, tcam);
		toMat3f(0, 0, zrotation, rcam);

		Mat terrsum = Mat::zeros(3,1,CV_32FC1);
		Mat rerrsum = Mat::zeros(3,1,CV_32FC1);
		Mat maxvec;
		float max = 0;
		Mat rmaxvec;
		float rmax = 0;
		float rzmax = 0;
		float xymax = 0;
		Mat xymaxvec;
		for(int i=0;i<repetitions;i++) {
			singleTestMultiMarker(markerSize, camParams, rcam, tcam, poseAlgo, regulationStrategy, multiMarkerStrategy,
					terr, rerr, detectionError, poseError, camRotation, camMovement, detectedMarkers);
			terrsum += abs(terr);
			rerrsum += abs(rerr);
			if(dist(terr) > 500) {
				cout << "ATTENTION ATTENTION: " << endl;
				cout << "terr: " << terr << endl;
				cout << endl << endl << endl;
				exit(0);
			}
			if(dist(terr) > max) {
				max = dist(terr);
				terr.copyTo(maxvec);
			}
			if(dist(rerr) > rmax) {
				rmax = dist(rerr);
				rerr.copyTo(rmaxvec);
			}
			if(dist2f(terr.rowRange(0,2)) > xymax) {
				terr.rowRange(0,2).copyTo(xymaxvec);
				xymax = dist2f(xymaxvec);
			}
			if(abs(rerr.at<float>(2)) > abs(rzmax)) {
				rzmax = abs(rerr.at<float>(2));
			}
		}
		terr = terrsum / (float)repetitions;
		rerr = rerrsum / (float)repetitions;

		cout << endl;
		cout << "AVG: "<<endl;
		cout << "terr: " << terr << endl;
		cout << "rerr: " << rerr << endl;
		cout << "dist(terr):" << dist(terr) << endl;
		cout << "dist(rerr):" << dist(rerr) << endl;
		cout << endl;
		cout << "MAX: "<<endl;
		cout << "terr: " << maxvec << endl;
		cout << "dist(terr):" << max << endl;
		cout << "rerr: " << rmaxvec << endl;
		cout << "dist(rerr):" << rmax << endl;

		maxterr = maxvec;
		maxrerr = rmaxvec;
		maxtxyerr = xymaxvec;
		maxrzerr = rzmax;

	}
};

#endif /* SYNTHETICTESTS_HPP_ */

/*
 * PoseEstimator.hpp
 *
 *  Created on: Jan 8, 2013
 *      Author: Bigontina
 */

#ifndef POSEESTIMATOR_HPP_
#define POSEESTIMATOR_HPP_

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/calib3d/calib3d_c.h>
#include <demonstrator/localizor/RobustPlanarPose/RPP.h>
#include <PositCoplanar/PositCoplanar.hpp>

#include <PoseEstimationAlgorithm.hpp>
#include <utils3d.hpp>

/*
 * This class executes the specified PoseEstimationAlgorithm
 * when getPose is called. It requires a set of 2d image points,
 * as well as a set of their corresponding 3d model points.
 */
class PoseEstimator {

public:

	int algo;
	Mat intrinsics;
	Mat distortion;

	PoseEstimator(Mat & intrinsics, PoseEstimationAlgorithm algo = PoseEstimationAlgorithm::ITERATIVE) {
		this->distortion = Mat::zeros(4,1,CV_32F);
		this->algo = algo;
		this->intrinsics = intrinsics;
	}

	float focalLength() {
		return((this->intrinsics.at<float>(0,0)+this->intrinsics.at<float>(1,1))/ 2.0f);
	}

	void getPose(cv::Mat modelPoints, cv::Mat imagePoints, cv::Mat & tvec, cv::Mat & rvec, bool extrinsicGuess=false) {
		switch(this->algo){
		case PoseEstimationAlgorithm::ITERATIVE:
			//cout << "PoseEstimationAlgorithm: ITERATIVE" << endl;
			getPoseIterative(modelPoints, imagePoints, tvec, rvec, extrinsicGuess);
			break;
		case PoseEstimationAlgorithm::P3P:
			//cout << "PoseEstimationAlgorithm: P3P" << endl;
			getPoseP3P(modelPoints, imagePoints, tvec, rvec, extrinsicGuess);
			break;
		case PoseEstimationAlgorithm::EPNP:
			//cout << "PoseEstimationAlgorithm: EPNP" << endl;
			getPoseEPNP(modelPoints, imagePoints, tvec, rvec, extrinsicGuess);
			break;
		case PoseEstimationAlgorithm::POSIT:
			//cout << "PoseEstimationAlgorithm: POSIT" << endl;
			getPosePOSIT(modelPoints, imagePoints, tvec, rvec);
			break;
		case PoseEstimationAlgorithm::RPPAlgo:
			//cout << "PoseEstimationAlgorithm: RPP" << endl;
			getPoseRPP(modelPoints, imagePoints, tvec, rvec);
			break;
		case PoseEstimationAlgorithm::PLANARPOSIT:
			//cout << "PoseEstimationAlgorithm: PLANARPOSIT" << endl;
			getPosePositCoplanar(modelPoints, imagePoints, tvec, rvec);
			break;
		default:
			//cout << "PoseEstimationAlgorithm: ITERATIVE (default)" << endl;
			getPoseIterative(modelPoints, imagePoints, tvec, rvec, extrinsicGuess);
			break;
		}
	}

	void unapplyIntrinsics(const Mat & points, const Mat & intrinsics, Mat & dstpoints, bool removeLastRow=true) {
		Mat points2h, points3;
		addOnesRow(points,points2h);
		points3 = intrinsics.inv() * points2h;
		normHomogeneous2(points3, dstpoints, removeLastRow);
	}

	/*
	 * OpenCV Iterative
	 */
	void getPoseIterative(const cv::Mat objectPoints, const cv::Mat imagePoints, cv::Mat & tvec, cv::Mat & rvec, bool extrinsicGuess) {
		printtype(objectPoints);
		printtype(imagePoints);
	    cv::Mat raux,taux;
	    bool res = cv::solvePnP(objectPoints.t(), imagePoints.t(), intrinsics, distortion, raux, taux, extrinsicGuess, cv::ITERATIVE);
	    raux.convertTo(rvec, CV_32F);
	    taux.convertTo(tvec ,CV_32F);

	    // make sure the results are not misinterpreted (values might be there from last time)
	    if(!res) {
	    	tvec = Mat();
	    	rvec = Mat();
	    }
	}

	/*
	 * OpenCV P3P
	 * This method was developed by Xia-Shan Gao et al. and called CASSC in their paper.
	 */
	void getPoseP3P(const cv::Mat objectPoints, const cv::Mat imagePoints, cv::Mat & tvec, cv::Mat & rvec, bool extrinsicGuess) {
		if(objectPoints.cols != 4) {
			cerr << " ATTENTION: P3P requires exactly 4 points! "<<objectPoints.cols<<" given!" <<endl;
			cerr << "            Taking only first 4" <<endl;
		}

		cv::Mat raux,taux;

		vector<Point2f> ipoints;
		vector<Point3f> opoints;
		matToPoints3f(objectPoints.colRange(0,4), opoints);
		matToPoints2f(imagePoints.colRange(0,4), ipoints);

	    bool res = cv::solvePnP(opoints, ipoints, intrinsics, distortion, raux, taux, extrinsicGuess, cv::P3P);
	    raux.convertTo(rvec, CV_32F);
	    taux.convertTo(tvec ,CV_32F);

	    // make sure the results are not misinterpreted (values might be there from last time)
	    if(!res) {
	    	tvec = Mat();
	    	rvec = Mat();
	    }
	}

	/*
	 * OpenCV EPNP
	 * Originally developed by Lepetit et al.
	 */
	void getPoseEPNP(const cv::Mat objectPoints, const cv::Mat imagePoints, cv::Mat & tvec, cv::Mat & rvec, bool extrinsicGuess) {
		cv::Mat raux,taux, opoints,ipoints;
		mat3fRowsToDepth(objectPoints, opoints);
		mat2fRowsToDepth(imagePoints, ipoints);

		opoints.convertTo(opoints, CV_64FC3);
		ipoints.convertTo(ipoints, CV_64FC2);

		bool res = cv::solvePnP(opoints, ipoints, intrinsics, distortion, raux, taux, extrinsicGuess, cv::EPNP);
		raux.convertTo(rvec, CV_32F);
		taux.convertTo(tvec ,CV_32F);

	    // make sure the results are not misinterpreted (values might be there from last time)
	    if(!res) {
	    	tvec = Mat();
	    	rvec = Mat();
	    }
	}

	/*
	 * OpenCV implementation of POSIT algorithm
	 * Algorithm originally developed by DeMenthon & Davis.
	 */
	void getPosePOSIT(const cv::Mat objectPoints, const cv::Mat imagePoints, cv::Mat & tvec, cv::Mat & rvec) {
		Mat rmat;
		float frot[9];
		float ftra[3];
		cv::TermCriteria criteria = cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 100, 1.0e-4f);

		Mat imagePointsUn;
		unapplyIntrinsics(imagePoints, intrinsics, imagePointsUn);

		vector<CvPoint3D32f> opoints;
		vector<CvPoint2D32f> ipoints;
		matToPoints3D32f(objectPoints, opoints);
		matToPoints2D32f(imagePointsUn, ipoints);

		CvPoint2D32f* ipointsdata = ipoints.data();

		CvPOSITObject *positObject = cvCreatePOSITObject(&opoints[0], objectPoints.cols);
		cvPOSIT( positObject, ipointsdata, 1/*this->focalLength()*/, criteria, frot, ftra );

		toMat3f(ftra[0],ftra[1],ftra[2],tvec);
		rmat = Mat(3,3,CV_32FC1,frot);
		fillMat3f(frot[0], frot[1], frot[2], rmat, 0);
		fillMat3f(frot[3], frot[4], frot[5], rmat, 1);
		fillMat3f(frot[6], frot[7], frot[8], rmat, 2);
		cv::Rodrigues(rmat, rvec);

		// tvec gives the distance to the fist point! why?!?
		tvec -= objectPoints.col(0);
	}

	/*
	 * RPP algorithm by Schweighofer and Pinz
	 * Using implementation by Nghia Ho
	 */
	void getPoseRPP(const cv::Mat objectPoints, const cv::Mat imagePoints, cv::Mat & tvec, cv::Mat & rvec) {
	    Mat rotation;
	    Mat translation;
	    int iterations;
	    double obj_err;
	    double img_err;
	    Mat ipoints, opoints;

	    //normalize points
	    unapplyIntrinsics(imagePoints, intrinsics, ipoints, false);

	    ipoints.convertTo(ipoints, CV_64F);
	    objectPoints.convertTo(opoints, CV_64F);

	    RPP::Rpp(opoints, ipoints, rotation, translation, iterations, obj_err, img_err);

		Mat iprts_data = ipoints;
		Mat model_data = opoints;

		tvec = translation;
		cv::Rodrigues(rotation, rvec);
		tvec.convertTo(tvec, CV_32F);
		rvec.convertTo(rvec, CV_32F);
	}

	/*
	 * Extension of the POSIT algorithm to work with coplanar points
	 * Developed by Oberkampf et al.
	 * Using implementation by Oberkampf
	 */
	void getPosePositCoplanar(const cv::Mat objectPoints, const cv::Mat imagePoints, cv::Mat & tvec, cv::Mat & rvec) {
		long int np = objectPoints.cols;
		Mat imagePointsT, objectPointsT, imagePointsDblT, objectPointsDblT;
		Mat tvecDbl, rmatDbl, rmat;

		// unapply intrinsics:
		Mat imagePointsUn;
		unapplyIntrinsics(imagePoints, intrinsics, imagePointsUn, true);

		cv::transpose(imagePointsUn, imagePointsT);
		cv::transpose(objectPoints, objectPointsT);

		imagePointsT.convertTo(imagePointsDblT, CV_64FC1);
		objectPointsT.convertTo(objectPointsDblT, CV_64FC1);

		double ** ipoints;
		double ** opoints;
		getMatData(imagePointsDblT, ipoints);
		getMatData(objectPointsDblT, opoints);

		double r[3][3], rot[3][3];
		double t[3];
		double noise = 0;
		double nbsol;
		PositCoplanar::Composit(np, ipoints, opoints, 1/*focalLength()*/, r, t, noise, rot, &nbsol);
		tvec = Mat(3,1,CV_64FC1, t);
		rmat = Mat(3,3,CV_64FC1, r);
		cv::Rodrigues(rmat, rvec);
		tvec.convertTo(tvec, CV_32F);
		rvec.convertTo(rvec, CV_32F);
	}
};


#endif /* POSEESTIMATOR_HPP_ */

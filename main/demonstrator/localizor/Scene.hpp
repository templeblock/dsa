/*
 * Scene.hpp
 *
 *  Created on: 20.02.2013
 *      Author: Bigontina
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <opencv2/opencv.hpp>
#include <Localizor/utils3d.hpp>
#include <Localizor/utils.hpp>
#include <MarkerUtils.hpp>

using namespace cv;

/*
 * Responsible for drawing a 3D scene. It can draw
 * the marker image at a defined pose, and the
 * marker or the camera as wireframes.
 */
class Scene {

private:
	Mat P;
	Mat screen;
	ViewSimulator sim;
	Mat intrinsics;
	bool orthogonal;

public:

	Scene(Mat intrinsics, Mat viewPosition, Mat viewRotation, Size screenSize, bool orthogonal=false) {
		this->intrinsics = intrinsics;
		this->screen = Mat::zeros(screenSize, CV_8UC3);
		this->orthogonal = orthogonal;
		setViewPose(viewPosition, viewRotation);
		sim = ViewSimulator();
	}

	Scene(Mat intrinsics, Mat viewPosition, Mat viewRotation, Mat & screen, bool orthogonal=false) {
		this->intrinsics = intrinsics;
		this->screen = screen;
		this->orthogonal = orthogonal;
		setViewPose(viewPosition, viewRotation);
		sim = ViewSimulator();
	}

	void setViewPose(Mat viewPosition, Mat viewRotation) {
		Mat F;
		generateFlipMatrix(screen.rows, F);
		generateProjectionMatrix(viewRotation, viewPosition, F*intrinsics, P, true);

		if(orthogonal) {
			Mat tmp = Mat::eye(3,3,CV_32FC1);
			generateProjectionMatrix(viewRotation, viewPosition, tmp, P, true);
			P.at<float>(2,0) = 0;
			P.at<float>(2,1) = 0;
			P.at<float>(2,2) = 0;
			P.at<float>(2,3) = 1;
			P = intrinsics * P;
		}
	}

	void clearScreen() {
		this->screen = Mat::zeros(screen.size(), CV_8UC3);
	}

	void drawMarkerImage(Mat & markerimg1, float markersize, Mat position, Mat rotation) {
		Mat model2,model2h,model3h,image3,image2h,T;

		Mat markerimg;
		flip(markerimg1, markerimg, 0);

		generateTranformationMatrix(rotation, position, T);

		genMarkerPoints(markersize, model3h, true);
		model2 = model3h.rowRange(0,2);
		addOnesRow(model2, model2h);

		image3 = P*T*model3h;
		normHomogeneous2(image3, image2h, false);

		Mat image2 = image2h.rowRange(0,2);

		Mat H = findHomography(model2.t(), image2.t());
		H.convertTo(H, CV_32FC1);

		Mat imageH;
		sim.normImageHomo(markerimg.cols, markerimg.rows, markersize, imageH);

		Mat newH = H*imageH;

		warpImage(markerimg, newH, screen);
	}

	void drawMarkerWire(float size, Mat position, Mat rotation, string text="", Scalar color=Scalar(128,0,0)) {
		Mat image3, image2h, model3h, T;

		generateTranformationMatrix(rotation, position, T);
		genMarkerPoints(size, model3h, true);

		image3 = P*T*model3h;
		normHomogeneous2(image3, image2h, true);

		Point p2;
		Point p1(image2h.at<float>(0,3), image2h.at<float>(1,3));
		for(int i=0;i<4;i++) {
			Point p2(image2h.at<float>(0,i), image2h.at<float>(1,i));
			cv::line(screen, p1, p2, color, 2);
			p1 = p2;
		}
		cv::putText(screen, text, p1, 0, 2, color, 2);

	}

	void drawCameraWire(Mat position, Mat rotation, float scale=10, Scalar color=Scalar(255,0,255)) {
		Mat p = Mat(3, 7, CV_32FC1);
		Mat l = Mat::zeros(2, 9, CV_8UC1);

		fillMat3f( 0, 0, 0, p, 0);
		fillMat3f(-1, 1, 2, p, 1);
		fillMat3f( 1, 1, 2, p, 2);
		fillMat3f( 1,-1, 2, p, 3);
		fillMat3f(-1,-1, 2, p, 4);
		fillMat3f( 0, 0.5, 2, p, 5);
		fillMat3f( 0, 1.5, 2, p, 6);

		fillMat2b(0,1,l,0);
		fillMat2b(0,2,l,1);
		fillMat2b(0,3,l,2);
		fillMat2b(0,4,l,3);

		fillMat2b(1,2,l,4);
		fillMat2b(2,3,l,5);
		fillMat2b(3,4,l,6);
		fillMat2b(4,1,l,7);

		fillMat2b(5,6,l,8);

		Mat pscaled = p*scale;

		Mat T, p2d;
		generateTranformationMatrix(rotation, position, T);
		project(P*T, pscaled, p2d);

		drawLines(screen, p2d, l, color, 1);
	}

	Mat* getScreen() {
		return(&screen);
	}

};


#endif /* SCENE_HPP_ */

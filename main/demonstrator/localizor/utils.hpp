/*
 * utils.hpp
 *
 *  Created on: Jan 8, 2013
 *      Author: Bigontina
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/core/types_c.h>
#include <consts.hpp>

using namespace std;
using namespace cv;

/*
 * Provides various functions, many to ease
 * working with OpenCV. Often just shortcuts
 * to keep code simple and readable.
 */


void printtype(const cv::Mat & m) {
	//#define CV_8U   0
	//#define CV_8S   1
	//#define CV_16U  2
	//#define CV_16S  3
	//#define CV_32S  4
	//#define CV_32F  5
	//#define CV_64F  6
	//#define CV_USRTYPE1 7

	int bits = pow(2,(3+ m.depth()/2));
	char* typetag;
	if(m.depth() == 0) typetag = "U";
	if(m.depth() == 1) typetag = "S";
	if(m.depth() == 2) typetag = "U";
	if(m.depth() == 3) typetag = "S";
	if(m.depth() == 4) typetag = "S";
	if(m.depth() == 5) typetag = "F";
	if(m.depth() == 6) typetag = "F";

	printf("CV_%d%sC%d\n", bits, typetag, m.channels());
}

/* Mat* emptyCopy(Mat & m) {
	return new Mat(cvCreateMat(m.rows, m.cols, m.type()));
} */

void fillMat2f(float x, float y, Mat & m, int col = 0) {
	m.at<float>(0,col) = x;
	m.at<float>(1,col) = y;
}

void fillMat3f(float x, float y, float z, Mat & m, int col = 0) {
	m.at<float>(0,col) = x;
	m.at<float>(1,col) = y;
	m.at<float>(2,col) = z;
}

void fillMat4f(float x, float y, float z, float w, Mat & m, int col = 0) {
	m.at<float>(0,col) = x;
	m.at<float>(1,col) = y;
	m.at<float>(2,col) = z;
	m.at<float>(3,col) = w;
}

void fillMat2b(uint8_t a, uint8_t b, Mat & m, int col = 0) {
	m.at<uint8_t>(0,col) = a;
	m.at<uint8_t>(1,col) = b;
}

void point3fToMat(cv::Point3f & p, Mat & m) {
	m = Mat(3,1,CV_32FC1);
	fillMat3f(p.x, p.y, p.z, m);
}

void mat2fToPoints(Mat & m, vector<Point2f> & points) {
	for(int i=0;i<m.cols;i++) {
		Point2f p = Point2f(m.at<float>(0,i), m.at<float>(1,i));
		points.push_back(p);
	}
}

void toMat(vector<cv::Point2f> & ps, Mat & m) {
	m = Mat(2, ps.size(), CV_32F);
	for(int i=0;i<ps.size();i++) {
		fillMat2f(ps[i].x, ps[i].y, m, i);
	}
}

void fillMat(vector<cv::Point2f> & ps, Mat m) {
	for(int i=0;i<ps.size();i++) {
		fillMat2f(ps[i].x, ps[i].y, m, i);
	}
}

void matToPoints3f(const Mat & m, vector<cv::Point3f> & ps) {
	ps = vector<cv::Point3f>(m.cols);
	for(int i=0;i<m.cols;i++) {
		ps[i] = Point3f(m.at<float>(0,i), m.at<float>(1,i), m.at<float>(2,i));
	}
}

void matToPoints2f(const Mat & m, vector<cv::Point2f> & ps) {
	ps = vector<cv::Point2f>(m.cols);
	for(int i=0;i<m.cols;i++) {
		ps[i] = Point2f(m.at<float>(0,i), m.at<float>(1,i));
	}
}

void matToPoints3D32f(const Mat & m, vector<CvPoint3D32f> & ps) {
	ps = vector<CvPoint3D32f>(m.cols);
	for(int i=0;i<m.cols;i++) {
		ps[i] = cvPoint3D32f(m.at<float>(0,i), m.at<float>(1,i), m.at<float>(2,i));
	}
}

void matToPoints2D32f(const Mat & m, vector<CvPoint2D32f> & ps) {
	ps = vector<CvPoint2D32f>(m.cols);
	for(int i=0;i<m.cols;i++) {
		ps[i] = cvPoint2D32f(m.at<float>(0,i), m.at<float>(1,i));
	}
}

void toMat3f(float x, float y, float z, Mat & m) {
    m.create(3,1,CV_32FC1); 
   // m = cv::Mat(cvCreateMat(3,1,CV_32FC1));
    
	fillMat3f(x,y,z,m);
}

void toMat2f(float x, float y, Mat & m) {
//	m = Mat(cvCreateMat(2,1,CV_32FC1));
    m.create(2,1,CV_32FC1);
	fillMat2f(x,y,m);
}

void addOnesRow(Mat src, Mat & dst) {
	dst = Mat::ones(src.rows+1, src.cols, src.type());
	Mat temp(dst.rowRange(0, src.rows));
	src.copyTo(temp);
}

void mat3fRowsToDepth(Mat src, Mat & dst) {
	dst = Mat::zeros(1, src.cols, CV_32FC3);
	for(int i=0;i<src.cols;i++) {
		for(int j=0;j<src.rows;j++) {
			dst.at<Vec3f>(0,i)[j] = src.at<float>(j,i);
		}
	}
}

void mat2fRowsToDepth(Mat src, Mat & dst) {
	dst = Mat(1, src.cols, CV_32FC2);
	for(int i=0;i<src.cols;i++) {
		for(int j=0;j<src.rows;j++) {
			dst.at<Vec2f>(0,i)[j] = src.at<float>(j,i);
		}
	}
}

void getMatData(Mat & m, double** & data) {
	double** d = new double*[m.rows]();
	for(int i=0;i<m.rows;i++) {
		double* e = new double[m.cols]();
		for(int j=0;j<m.cols;j++) {
			e[j] = m.at<double>(i,j);
		}
		d[i] = e;
	}
	data = (double**)d;
}

void normHomogeneousSingle(Mat src, Mat & dst, int col = 0, bool removeLastRow = false) {
	int rows = removeLastRow ? src.rows-1 : src.rows;
	float last = src.at<float>(src.rows-1,col);
	for(int i=0;i<rows;i++) {
		dst.at<float>(i,col) = src.at<float>(i,col) / last;
	}
}

void normHomogeneous2(Mat src, Mat & dst, bool removeLastRow=false, bool initDst=true) {
	if(initDst) {
		int rows = removeLastRow ? src.rows-1 : src.rows;
		dst = Mat(rows, src.cols, src.type());
	}
	for(int i=0;i<src.cols;i++) {
		normHomogeneousSingle(src, dst, i, removeLastRow);
	}
}

void normHomogeneous(Mat src, Mat * dst, bool keepLastRow=false) {
	int r = keepLastRow ? src.rows : src.rows - 1;
	*dst = Mat(src.rows, src.cols, CV_8UC3);
	convertPointsHomogeneous(src,*dst);
}

void drawLines(Mat & img, Mat & points, Mat & connections, Scalar color, int thickness=2) {
	for(int i=0;i<connections.cols;i++) {
		Mat p1m = points.col(connections.at<uint8_t>(0,i));
		Mat p2m = points.col(connections.at<uint8_t>(1,i));
		Point2f p1 = Point2f(p1m.at<float>(0,0), p1m.at<float>(1,0));
		Point2f p2 = Point2f(p2m.at<float>(0,0), p2m.at<float>(1,0));
		line(img, p1, p2, color, thickness);
	}
}

float sqdist(Mat vec) {
	return( pow(vec.at<float>(0),2) + pow(vec.at<float>(1),2) + pow(vec.at<float>(2),2) );
}

float dist(Mat vec) {
	return(sqrt(sqdist(vec)));
}

float sqdist2f(Mat vec){
	return( pow(vec.at<float>(0),2) + pow(vec.at<float>(1),2) );
}

float dist2f(Mat vec) {
	return (sqrt(sqdist2f(vec)));
}

void rotationDist(Mat rvec, Mat rvec_est, Mat & rerr) {
	Mat rmat, rmat_est;
	Rodrigues(rvec, rmat);
	Rodrigues(rvec_est, rmat_est);
	Rodrigues(rmat_est.t() * rmat, rerr);
}

float rad(float deg) {
	return(deg/180. * (float)M_PI);
}

float deg(float rad) {
	return(rad/(float)M_PI * 180.);
}

void rotateImage180(Mat & img) {
	for(int x = 0;x < img.cols; x++) {
		for(int y = 0; y < img.rows/2; y++) {
			Vec3b t = img.at<Vec3b>(img.rows-y-1, img.cols-x-1);
			img.at<Vec3b>(img.rows-y-1, img.cols-x-1) = img.at<Vec3b>(y,x);
			img.at<Vec3b>(y,x) = t;
		}
	}
}

void avg(Mat & m, Mat & out) {
	out = Mat::zeros(3,1,CV_32FC1);
	for(int i=0;i<m.cols;i++) {
		out += m.col(i);
	}
	out = out/m.cols;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return(false);
    str.replace(start_pos, from.length(), to);
    return(true);
}

bool replace(std::string& str, const std::string& from, const int to) {
	stringstream s;
	s << to;
	return replace(str, from, s.str());
}

#endif /* UTILS_HPP_ */

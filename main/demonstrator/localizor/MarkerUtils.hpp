/*
 * MarkerUtils.hpp
 *
 *  Created on: 05.05.2013
 *      Author: user
 */

#ifndef MARKERUTILS_HPP_
#define MARKERUTILS_HPP_

#include <aruco.h>
#include <arucofidmarkers.h>
#include <opencv2/opencv.hpp>

#include <edgeldetector.h>
#include <linesegment.h>
#include <buffer.h>
#include <opencv2/highgui/highgui_c.h>
using namespace cv;

bool findMarkers(cv::Mat &image, std::vector<aruco::Marker> &markers, aruco::CameraParameters camparams, float threshBlockSize, float threshSubtract, float minMarker, float maxMarker) {
   // New method
   /*Buffer* buffer = new Buffer();
   EdgelDetector* edgelDetector = new EdgelDetector();
   edgelDetector->debugDrawLineSegments(false);
   edgelDetector->debugDrawPartialMergedLineSegments(false);
   edgelDetector->debugDrawMergedLineSegments(false);
   edgelDetector->debugDrawExtendedLineSegments(false);
   edgelDetector->debugDrawSectors(false);
   edgelDetector->debugDrawSectorGrids(false);
   edgelDetector->debugDrawEdges(true);
   edgelDetector->debugDrawCorners(true);
   edgelDetector->debugDrawMarkers(true);


   
   //namedWindow("test",WINDOW_AUTOSIZE);
   cv::Mat im2 = cv::imread("raspicam_cv_image.jpg");
   IplImage im = im2;
   
   //cv::imwrite("FindMarker_image.jpg",im2);
   //cvShowImage("test",im);
   buffer->setBuffer((unsigned char *) im.imageData, im.width, im.height);
   edgelDetector->setBuffer(buffer);
   // test 
   int imw = im.width;
   int imh = im.height;
   int bufw = buffer->getWidth();
   int bufh = buffer->getHeight();
   clock_t cs = clock(); 
   std::vector<ARMarker> ARMarkers = edgelDetector->findMarkers();

   clock_t c1=clock();
	std::cout<<"Edgel detector:"<<std::fixed<<std::setprecision(2)<<((float)c1-cs)/CLOCKS_PER_SEC<<"\n";

   
   //cvSaveImage("testMarker_image.jpg",&im);

   delete buffer;
   delete edgelDetector; */
   //cvReleaseImage(im);
   // end New method
	aruco::MarkerDetector detector;
	detector.setThresholdMethod(aruco::MarkerDetector::CANNY);
	detector.setThresholdParams(threshBlockSize, threshSubtract);
	detector.setMinMaxSize(minMarker, maxMarker);
   detector.setDesiredSpeed(2);
	detector.detect(image, markers, camparams, 0); // last argument markerSize == 0
												   // means "perform no pose estimation
	
   std::cout << std::endl << "markers.size() is " << markers.size() << std::endl;
	return(markers.size() > 0);
}

bool findMarkers(cv::Mat &image, std::vector<aruco::Marker> &markers, aruco::CameraParameters camparams) {
	return(findMarkers(image, markers, camparams, 21.0f, 0.0f, 0.001f, 0.5f));
}

void genMarkerPoints(float markerSize, Mat & m, bool homogeneous=false) {
	float halfSize=markerSize/2.0f;
	int rows = homogeneous ? 4 : 3;

	m = Mat(rows,4,CV_32F);
	m.at<float>(0,3)=-halfSize;
	m.at<float>(1,3)=-halfSize;
	m.at<float>(2,3)=0;
	m.at<float>(0,2)=halfSize;
	m.at<float>(1,2)=-halfSize;
	m.at<float>(2,2)=0;
	m.at<float>(0,1)=halfSize;
	m.at<float>(1,1)=halfSize;
	m.at<float>(2,1)=0;
	m.at<float>(0,0)=-halfSize;
	m.at<float>(1,0)=halfSize;
	m.at<float>(2,0)=0;

	if(homogeneous) {
		m.row(3)=1;
	}
}


#endif /* MARKERUTILS_HPP_ */

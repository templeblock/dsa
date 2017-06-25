/*
 * Display.hpp
 *
 *  Created on: Jan 8, 2013
 *      Author: Bigontina
 */

#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <aruco.h>
#include <opencv2/highgui/highgui.hpp>

using namespace aruco;

/*
 * Helper class to ease writing text on an image and display it.
 * Any output can be added to 'textstream' using '<<' and will be drawn
 * onto the image when 'show' is called. Breaks can be used as well by
 * writing '\n'.
 */
class Display {

public:

	Mat* displayImage;
	int textRow;
	std::stringstream textstream;
	string windowName;

	void init(string windowName = "Camera") {
		this->windowName = windowName;
		cvStartWindowThread();
		cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	}

	void drawTextInfo(const char* text) {
		drawTextInfoR(text, ++textRow);
	}

	void drawTextInfoR(const char* text,int row) {
		int top = 40;
		int left = 20;
		int rowSize = 30;
		putText(*displayImage, text, cvPoint(left,top+row*rowSize), 0,0.5,cvScalar(0,0,0),3);
		putText(*displayImage, text, cvPoint(left,top+row*rowSize), 0,0.5,cvScalar(0,0,200),2);
	}

	void setImage(Mat* img) {
		displayImage = img;
		textRow = 0;
	}

	void show() {
		string line;
		while(std::getline(textstream, line, '\n')) {
			drawTextInfo(line.c_str());
		}
		textstream.str("");
		textstream.clear();

		cv::imshow(windowName, *displayImage);
	}
};

#endif /* DISPLAY_HPP_ */

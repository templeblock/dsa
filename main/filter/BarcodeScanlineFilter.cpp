/*
 * BarcodeScanlineFilter.cpp
 *
 * 	See
 *
 *	barcode_06-xsimur01.pdf
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "BarcodeScanlineFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::BarcodeScanlineFilter::BarcodeScanlineFilter() {
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BarcodeScanlineFilter::~BarcodeScanlineFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BarcodeScanlineFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::BarcodeScanlineFilter::apply() {
	if(isReady()) {
		size_t hBlocks = 40; // horizontal blocks
		size_t ksize = src->cols/hBlocks;

		hBlocks = src->cols/ksize;
		size_t vBlocks = src->rows/ksize;

		cv::cvtColor(*src, dst, CV_GRAY2RGB);
		int strongThreshold = std::max((int)ksize/8, 2);
		int weakThreshold = 2;

		for(size_t i = 0; i < vBlocks; i++) {	// in all rows
			for(size_t j = 0; j < hBlocks; j++) {	// in all cols

				std::vector<int> extrema = std::vector<int>();
				int count[4];

				int iBase = i*ksize;
				int jBase = j*ksize;

				//std::cout << jBase << "," << iBase << "; " << jBase+ksize << "," << iBase+ksize << std::endl;
				//cv::Mat subImage = (*src)(cv::Rect(jBase, iBase, ksize, ksize));
				//imshow("Box", subImage);
				//subImage.release();
				//cv::waitKey(0);

				// C1 (horizontal)
				int iLineStart = iBase + ksize/2;
				int jLineStart = jBase;

				int current = src->at<uchar>(iLineStart, jLineStart);
				int next = src->at<uchar>(iLineStart, ++jLineStart);
				int prevDirection = direction(current, next);

				extrema.push_back(current);
				for(size_t k = 1; k < ksize-1; k++) {
					current = src->at<uchar>(iLineStart, jLineStart);
					next = src->at<uchar>(iLineStart, ++jLineStart);
					int curDirection = direction(current, next);
					if(curDirection != prevDirection) {
						prevDirection = curDirection;
						extrema.push_back(current);
					}
				}
				count[0] = countChanges(extrema);
				extrema.clear();

				// C2 (diagnonal)
				iLineStart = iBase;
				jLineStart = jBase + ksize;

				current = src->at<uchar>(iLineStart, jLineStart);
				next = src->at<uchar>(++iLineStart, --jLineStart);
				prevDirection = direction(current, next);

				extrema.push_back(current);
				for(size_t k = 1; k < ksize-1; k++) {
					current = src->at<uchar>(iLineStart, jLineStart);
					next = src->at<uchar>(++iLineStart, --jLineStart);
					int curDirection = direction(current, next);
					if(curDirection != prevDirection) {
						prevDirection = curDirection;
						extrema.push_back(current);
					}
				}
				count[1] = countChanges(extrema);
				extrema.clear();

				// C3 (vertical)
				iLineStart = iBase;
				jLineStart = jBase + ksize/2;

				current = src->at<uchar>(iLineStart, jLineStart);
				next = src->at<uchar>(++iLineStart, jLineStart);
				prevDirection = direction(current, next);

				extrema.push_back(current);
				for(size_t k = 1; k < ksize-1; k++) {
					current = src->at<uchar>(iLineStart, jLineStart);
					next = src->at<uchar>(++iLineStart, jLineStart);
					int curDirection = direction(current, next);
					if(curDirection != prevDirection) {
						prevDirection = curDirection;
						extrema.push_back(current);
					}
				}
				count[2] = countChanges(extrema);
				extrema.clear();

				// C4 (diagnonal)
				iLineStart = iBase;
				jLineStart = jBase;

				current = src->at<uchar>(iLineStart, jLineStart);
				next = src->at<uchar>(++iLineStart, ++jLineStart);
				prevDirection = direction(current, next);

				extrema.push_back(current);
				for(size_t k = 1; k < ksize-1; k++) {
					current = src->at<uchar>(iLineStart, jLineStart);
					next = src->at<uchar>(++iLineStart, ++jLineStart);
					int curDirection = direction(current, next);
					if(curDirection != prevDirection) {
						prevDirection = curDirection;
						extrema.push_back(current);
					}
				}
				count[3] = countChanges(extrema);
				extrema.clear();

				int max, max1, max2;
				int orientation, orientation1, orientation2;
				if(count[0] > count[2]) {
					max1 = count[0] - count[2];
					orientation1 = 0;
				} else {
					max1 = count[2] - count[0];
					orientation1 = 2;
				}
				if(count[1] > count[3]) {
					max2 = count[1] - count[3];
					orientation2 = 1;
				} else {
					max2 = count[3] - count[1];
					orientation1 = 3;
				}

				if(max1 > max2) {
					max = max1;
					orientation = orientation1;
				} else {
					max = max2;
					orientation = orientation2;
				}

				if(max > strongThreshold) {
					cv::rectangle(dst, cv::Point(jBase, iBase), cv::Point(jBase+ksize, iBase+ksize), cv::Scalar(0,255,0), -1);
					int i, j;
					switch(orientation) {
					case 0:
						i = iBase+ksize/2;
						j = jBase+ksize;
						break;
					case 1:
						i = iBase+ksize;
						j = jBase+ksize;
						break;
					case 2:
						i = iBase+ksize;
						j = jBase+ksize/2;
						break;
					case 3:
						i = iBase+ksize;
						j = jBase;
						break;
					}
					dst.at<cv::Vec3b>(i, j) = cv::Vec3b(0,0,255);
				}
			}
		}
	}
}


int dsa::BarcodeScanlineFilter::countChanges(std::vector<int> &v) {
	const int T = 70;	// threshold
	int retval = 0;
	for(size_t i = 0; i < v.size()-1; i++) {
		if(abs(v[i]-v[i+1]) > T)
			retval++;
	}
	//std::cout << "changes=" << retval << std::endl;
	return(retval);
}

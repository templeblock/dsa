/*
 * BarcodeGradientFilter.cpp
 *
 * 	See
 *
 *	barcode_poster_taller_0.pdf
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "BarcodeGradientFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::BarcodeGradientFilter::BarcodeGradientFilter() {
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::BarcodeGradientFilter::~BarcodeGradientFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::BarcodeGradientFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::BarcodeGradientFilter::apply() {
	if(isReady()) {
		cv::Mat maxEnergyValues = cv::Mat(src->rows, src->cols, CV_8UC1);
		for(size_t i = 1; i < src->rows-1; i++) {	// in all rows
			for(size_t j = 1; j < src->cols-1; j++) {	// in all cols
				int center = src->at<uchar>(i, j);
				int left = src->at<uchar>(i, j-1);
				int right = src->at<uchar>(i, j+1);
				int top = src->at<uchar>(i-1, j);
				int bottom = src->at<uchar>(i+1, j);
				int horizontal = abs(left-center) + abs(right-center);
				int vertical = abs(top-center) + abs(bottom-center);
				maxEnergyValues.at<uchar>(i,j) = cv::saturate_cast<uchar>(horizontal - vertical);
			}
		}
		medianBlur(maxEnergyValues, maxEnergyValues, 3);
		cv::subtract(maxEnergyValues, cv::Scalar(100), maxEnergyValues);
		cv::imshow("Energy", maxEnergyValues);
	}
}
		/*size_t hBlocks = 80; // horizontal blocks
		size_t ksize = src->cols/hBlocks;

		hBlocks = src->cols/ksize;
		size_t vBlocks = src->rows/ksize;

		cv::Mat maxEnergyValues = cv::Mat(src->rows, src->cols, CV_8UC1);
		cv::Mat orientation = cv::Mat(vBlocks, hBlocks, CV_8UC1);

		//cv::cvtColor(*src, dst, CV_GRAY2RGB);

		for(size_t i = 0; i < vBlocks; i++) {	// in all rows
			for(size_t j = 0; j < hBlocks; j++) {	// in all cols
				int iBase = i*ksize;
				int jBase = j*ksize;

				// C1 (horizontal)
				int iLineStart = iBase + ksize/2;
				int jLineStart = jBase;

				int current = src->at<uchar>(iLineStart, jLineStart);
				int next = src->at<uchar>(iLineStart, ++jLineStart);

				int c1sum = 0;
				for(size_t k = 1; k < ksize-1; k++) {
					current = src->at<uchar>(iLineStart, jLineStart);
					next = src->at<uchar>(iLineStart, ++jLineStart);
					c1sum += abs(next - current);
				}

				// C2 (diagnonal)
				iLineStart = iBase;
				jLineStart = jBase + ksize;

				current = src->at<uchar>(iLineStart, jLineStart);
				next = src->at<uchar>(++iLineStart, --jLineStart);

				int c2sum = 0;
				for(size_t k = 1; k < ksize-1; k++) {
					current = src->at<uchar>(iLineStart, jLineStart);
					next = src->at<uchar>(++iLineStart, --jLineStart);
					c2sum += abs(next - current);
				}

				// C3 (vertical)
				iLineStart = iBase;
				jLineStart = jBase + ksize/2;

				current = src->at<uchar>(iLineStart, jLineStart);
				next = src->at<uchar>(++iLineStart, jLineStart);

				int c3sum = 0;
				for(size_t k = 1; k < ksize-1; k++) {
					current = src->at<uchar>(iLineStart, jLineStart);
					next = src->at<uchar>(++iLineStart, jLineStart);
					c3sum += abs(next - current);
				}

				// C4 (diagnonal)
				iLineStart = iBase;
				jLineStart = jBase;

				current = src->at<uchar>(iLineStart, jLineStart);
				next = src->at<uchar>(++iLineStart, ++jLineStart);

				int c4sum = 0;
				for(size_t k = 1; k < ksize-1; k++) {
					current = src->at<uchar>(iLineStart, jLineStart);
					next = src->at<uchar>(++iLineStart, ++jLineStart);
					c4sum += abs(next - current);
				}

				int max1, max2;
				UINT8 orientation1, orientation2;
				if(c1sum > c3sum) {		// horizontal > vertical => orientation angle 0° (1)
					max1 = c1sum - c3sum;
					orientation1 = 1;
				} else {	// vertical > horizontal => orientation 90° (3)
					max1 = c3sum - c1sum;
					orientation1 = 3;
				}
				if(c2sum > c4sum) {	// top-left->low-right > top-right->low-left => orientation 45° (2)
					max2 = c2sum - c4sum;
					orientation2 = 2;
				} else {
					// top-right->low-left > top-left->low-right => orientation 135° (4)
					max2 = c4sum - c2sum;
					orientation2 = 4;
				}

				int max;
				if(max1 > max2) {
					max = max1;
					orientation.at<UINT8>(i, j) = orientation1;
				} else {
					max = max2;
					orientation.at<UINT8>(i, j) = orientation2;
				}

				max -= 128;
				UINT8 value = cv::saturate_cast<UINT8>(max);

				cv::rectangle(maxEnergyValues, cv::Point(jBase, iBase), cv::Point(jBase+ksize, iBase+ksize), cv::Scalar(value), CV_FILLED);
				// candidate with orientation weakest line - 90°
				//uchar value = weakest*60;
				for(size_t i = 0; i < ksize; i++) {	// in all rows
					for(size_t j = 0; j < ksize; j++) {	// in all cols
						dst.at<uchar>(iBase+i,jBase+j) = value;
					}
				}
			}
		}
		cv::Mat dst2 = cv::Mat(src->rows, src->cols, CV_8UC1);
		medianBlur(maxEnergyValues, dst2, ksize-1);
		cv::imshow("Energy", dst2);*/

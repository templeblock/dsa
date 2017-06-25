/*
 * BarcodeSegmenter.cpp
 *
 * 	See
 *
 *	barcode_extraction_framework.pdf
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "BarcodeSegmenter.h"

dsa::BarcodeSegmenter::BarcodeSegmenter(const int ksize) {
	// these two options are the values recommended by the paper
	paper_prefilter_threshold_ = 32;
	paper_prefilter_operations_ksize_ = 5;

	ksize_ = ksize;
	windowSize_ = ksize_ * ksize_;

	minShort_ = ceil(100.0/ksize_);
	minLong_ = ceil(146.0/ksize_);
	minSize_ = minShort_ * minLong_; // 146x100 pixel (EAN8: 8digits * 7bars/digit * 2pixel/bar + 30% overhead

	darkThreshold_ = 85; // [0-84] is considered dark (first third in 0-255 range)
	lightThreshold_ = 171; // [171-255] is considered light (last third in 0-255 range)
	maxMediumRatio_ = 0.4;	// how big do we allow the remaining region to get

	// determine how close to a feasible rectangle this segment is
	maxPerimeterRatio_ = 1.25; // how close is the perimeter to that of our rotated rectangle?
	minAreaCoverage_ = 0.6;	// how close is the area to that of our rotated rectangle?
	maxSideLengthRatio_ = 13.0;	// do length and width suggest a feasible rectangle for a barcode
}

dsa::BarcodeSegmenter::~BarcodeSegmenter() {
}

void dsa::BarcodeSegmenter::prefilter(cv::Mat &src, cv::Mat &dst) {
	dst = cv::Mat(src.rows/ksize_, src.cols/ksize_, CV_8UC1);
	for(size_t i = 0; i < dst.rows; i++) {	// in all rows
	    UINT8* dstrow = dst.ptr<UINT8>(i);
		for(size_t j = 0; j < dst.cols; j++) {	// in all cols
			UINT8 minVal = 255, maxVal = 0;
			size_t kmin = i * ksize_;
			size_t kmax = kmin + ksize_;
			size_t lmin = j * ksize_;
			size_t lmax = lmin + ksize_;
			for(size_t k = kmin; k < kmax; k++) {
				UINT8* srcrow = src.ptr<UINT8>(k);
				for(size_t l = lmin; l < lmax; l++) {
					UINT8 value = srcrow[l];
					if(value < minVal)
						minVal = value;
					if(value > maxVal)
						maxVal = value;
				}
			}
			dstrow[j] = (UINT8)(maxVal - minVal);
		}
	}

	cv::Mat erosionElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(paper_prefilter_operations_ksize_, paper_prefilter_operations_ksize_));
	cv::Mat dilationElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(paper_prefilter_operations_ksize_, paper_prefilter_operations_ksize_));

	cv::Mat tmp;
	cv::blur(dst, tmp, cv::Size(paper_prefilter_operations_ksize_, paper_prefilter_operations_ksize_));
	cv::threshold(tmp, dst, paper_prefilter_threshold_, 255, cv::THRESH_BINARY);	// value from the paper. up for debate, my guess is it's too low
	cv::erode(dst, dst, erosionElement);
	cv::dilate(dst, dst, dilationElement);

	//cv::imshow("prefilter_dst", dst);
}

void dsa::BarcodeSegmenter::label(cv::Mat &src, cv::Mat &mask, std::vector<cv::Point> &min, std::vector<cv::Point> &max, std::vector<int> &size) {
	// implementation from the paper
	mask = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::Mat img2 = cv::Mat::zeros(src.rows, src.cols, CV_16UC1);	// beware we use 16-bit (unsigned short) per pixel here, as there can be more than 256 connected regions at first
	// connected component analysis
	// pass one
	UINT16 Label = 0;	// label no.
	std::vector<UINT16> L = std::vector<UINT16>(1, 0);	// equivalent list
	for(size_t i = 1; i < src.rows-1; i++) {
		UINT8* srcprevrow = src.ptr<UINT8>(i-1);
		UINT8* srcrow = src.ptr<UINT8>(i);
		UINT16* img2prevrow = img2.ptr<UINT16>(i-1);
		UINT16* img2row = img2.ptr<UINT16>(i);
		for(size_t j = 1; j < src.cols-1; j++) {
			if(srcrow[j] != 0) {	// not background
				// fixes an error in the paper, you actually have to use L[pixelvalue], as region designated by pixelvalue might have been merged in the "else" part below
				int topValue = srcprevrow[j];	// background for negative rows
				int leftValue = srcrow[j-1];	// background for negative cols
				if(topValue == 0 && leftValue == 0) {	// not connected => new element, new label
					Label++;
					L.push_back(Label);
					img2row[j] = (UINT16)Label;
				} else if (topValue == 0) {	// connected only to the left side
					UINT16 leftLabel = L[img2row[j-1]];
					img2row[j] = (UINT16)leftLabel;
				} else if(leftValue == 0) {	// connected only to the top side
					UINT16 topLabel = L[img2prevrow[j]];
					img2row[j] = (UINT16)topLabel;
				} else {	// connected to two components => merge both components into one
					UINT16 minLabel;
					UINT16 topLabel = L[img2prevrow[j]];
					UINT16 leftLabel = L[img2row[j-1]];
					if(topLabel > leftLabel) {
						img2row[j] = (UINT16)leftLabel;
						L[topLabel] = leftLabel;
					} else {
						img2row[j] = (UINT16)topLabel;
						L[leftLabel] = topLabel;
					}
				}
			}
		}
	}
	// pass one: process the equivalent List
	UINT16 NumComponent = 0;
	for(size_t i = 1; i < L.size(); i++) {
		if(L[i] == i) { // new component which was not merged to another with a lower number
			NumComponent++;
			L[i] = NumComponent;
		} else	// was merged, so use the number
			L[i] = L[L[i]];
	}
	//std::cout << L.size() << " NumComponent = " << NumComponent << std::endl;

	// pass two: assign final labels in the image
	size = std::vector<int>(NumComponent+1, 0);	// size of the segments
	min = std::vector<cv::Point>(NumComponent+1, cv::Point(INT_MAX, INT_MAX));	// segment upper left corner
	max = std::vector<cv::Point>(NumComponent+1, cv::Point(0, 0));	// segment lower right corner

	for(size_t i = 0; i < img2.rows; i++) {
		UINT16* img2row = img2.ptr<UINT16>(i);
		UINT8* maskrow = mask.ptr<UINT8>(i);
		for(size_t j = 0; j < img2.cols; j++) {
			UINT16 value = img2row[j];
			if(value != 0) {
				UINT16 component = L[value];
				if(j < min[component].x)
					min[component].x = j;
				if(i < min[component].y)
					min[component].y = i;
				if(j > max[component].x)
					max[component].x = j;
				if(i > max[component].y)
					max[component].y = i;

				size[component]++;
				// draw it into the mask
				maskrow[j] = (UINT8)component;
			}
		}
	}
}

void dsa::BarcodeSegmenter::enhanceComponent(cv::Mat &image, cv::Mat &mask, int compsize) {
	// normalize image
	cv::Mat normalized;
	normalize(image, normalized, 0, 255, cv::NORM_MINMAX, -1, mask);
	normalized.copyTo(image, mask);

	// increase brightness where necessary
	size_t npixels = image.rows * image.cols;
	int histogram[256] = {0};
	// calculate grayscale histogram
	for(size_t i = 0; i < npixels; i++)
		if(mask.data[i] > 0)
			histogram[image.data[i]]++;

	// we choose alpha in a way that would move 1/6 of all pixels in the range [1/2, 2/3] within the histogram into the last third.
	int begin = 0, end = 0;
	int beginPixels = compsize/2;
	int endPixels = 2*compsize/3;
	int pixels = 0;
	while((pixels += histogram[begin]) < beginPixels)
		begin++;
	end = begin + 1;
	while((pixels += histogram[end]) < endPixels)
		end++;
	float alpha = (float)end/(float)begin;
	image *= alpha;

	// sharpen image
	// determine how much we need to sharpen once image illumination was enhanced (RMS as contrast measure), can't do this together with histogram calculation, as it is computed after changes have been made to the image
	float maskedMean = mean(image, mask).val[0];
	float sum = 0.0f;
	for(size_t i = 0; i < npixels; i++) {
		if(mask.data[i] > 0) {
			float normdiff = 1.0f/255.0f * ((float)(image.data[i]) - maskedMean);
			sum += (normdiff * normdiff);
		}
	}
	float RMS = sqrtf(sum/npixels);

	/*imshow("sharpened", image);
	cv::waitKey();*/

	// sharpen when and as much as necessary (doesnt guarantee readability, but also doesnt screw up the image)
	if(RMS > 0.1 && RMS < 0.3) {	// RMS >= 1/3 is enough contrast < 0.1 is hopeless anyway
		double blurWeight = 20.0 * (1.0/3.0 - RMS);
		cv::Mat tmp;
		cv::GaussianBlur(image, tmp, cv::Size(3, 3), 0);
		cv::addWeighted(image, 1+blurWeight, tmp, -blurWeight, 0.0, image);
	}

	/*std::cerr << "RMS " << RMS << std::endl;
	imshow("sharpened", image);
	cv::waitKey();*/
}

void dsa::BarcodeSegmenter::segmentsFromComponents(cv::Mat &image, std::vector<dsa::Roi> &segments, cv::Mat &smask, std::vector<cv::Point> &min, std::vector<cv::Point> &max, std::vector<int> &size) {
	// the paper is not clear about the steps done from here on, so this is sort of my own interpretation of what they might have meant with their brief description
	// we need to find a good value here, depending on which barcode size can be actually read, since finding barcodes without being able to read their contents is of limited use
	for(size_t i = 1; i < min.size(); i++) {
		//std::cout << i << ": component size = " << size[i] << std::endl;
		cv::Rect boundingBox = cv::Rect(min[i], max[i]);
		// determine best case and ignore all candidates that do not even met the constraints then
		float bestPossibleShortSide = (boundingBox.height > boundingBox.width) ? boundingBox.width : boundingBox.height;	// = the shorter side of the bounding rectangle
		float bestPossibleLongSide = sqrtf(boundingBox.height*boundingBox.height + boundingBox.width*boundingBox.width);	// = the diagonal of the bounding rectangle
		if(size[i] >= minSize_ && bestPossibleLongSide >= minLong_ && bestPossibleShortSide >= minShort_) {
			cv::Mat mask = smask(boundingBox).clone();
			compare(mask, i, mask, cv::CMP_EQ);	// extract only the current component (rectangle might contain parts of others painted in a different color)
			cv::Mat maskContours = mask.clone();	// copy mask, as findContours will modify it

			// find contours to determine rotated rectangle
			std::vector<std::vector<cv::Point> > contours;
			std::vector<cv::Vec4i> hierarchy;
			findContours(maskContours, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

			if(contours.size()) {
				double contourPerimeter = 0.0;
				size_t maxContourIndex = 0;
				for(size_t j = 0; j < contours.size(); j++) {
					double length = arcLength(contours[j], true);
					if(length > contourPerimeter) {
						maxContourIndex = j;
						contourPerimeter = length;
					}
				}

				//cv::RotatedRect rotatedRect = cv::fitEllipse(cv::Mat(contours[maxContourIndex]));
				cv::RotatedRect rotatedRect = cv::minAreaRect(cv::Mat(contours[maxContourIndex]));

				// determine smaller and greater side length
				float longSide, shortSide;
				if(rotatedRect.size.height > rotatedRect.size.width) {
					longSide = rotatedRect.size.height;
					shortSide = rotatedRect.size.width;
				} else {
					longSide = rotatedRect.size.width;
					shortSide = rotatedRect.size.height;
				}
				double rectPerimeter = 2.0f * (longSide + shortSide);
				double perimeterRatio = (contourPerimeter > rectPerimeter) ? contourPerimeter/rectPerimeter : rectPerimeter/contourPerimeter;	// filters areas that have cuts and thus a longer perimeter (might be unnecessary)
				double areaCoverage = (double)size[i]/(longSide * shortSide);	// areas used by the segment within the rotated rectangle
				double sidelengthRatio = longSide/shortSide;
				//std::cout << "(longSide, shortSide, perimeterRatio, areaCoverage, sidelengthRatio) = (" << longSide << ", " << shortSide << ", " << perimeterRatio << ", " << areaCoverage << ", " << sidelengthRatio << ")" << std::endl;
				if(longSide >= minLong_ && shortSide >= minShort_ && perimeterRatio <= maxPerimeterRatio_ && areaCoverage >= minAreaCoverage_ && sidelengthRatio <= maxSideLengthRatio_) {
					// now we can work with full size image parts and mask
					min[i] = ksize_ * min[i];
					max[i] = ksize_ * max[i] + cv::Point(ksize_-1, ksize_-1);	// offset necessary since we have to include the additional 4
					size[i] = windowSize_ * size[i];

					// new boundingBox for full size image/mask
					boundingBox = cv::Rect(min[i], max[i]);
					cv::Mat roi = image(boundingBox);

					// draw rotated rectangle into mask to fill inner holes in the mask
					cv::Point2f vertices[4];
					rotatedRect.points(vertices);
					std::vector<cv::Point> pointList = std::vector<cv::Point>();
					for(size_t j = 0; j < 4; j++)
						pointList.push_back(vertices[j]);
					cv::fillConvexPoly(mask, pointList, cv::Scalar(255));

					// resize mask to original size (to fit roi)
					cv::Mat full_mask = cv::Mat::zeros(roi.rows, roi.cols, CV_8UC1);
					cv::resize(mask, full_mask, cv::Size(boundingBox.width, boundingBox.height), 0, 0, cv::INTER_NEAREST);

					// update roteted rectangle to match scaled image
					rotatedRect.center = 0.5f * cv::Point2f(boundingBox.width, boundingBox.height);
					rotatedRect.size.height *= ksize_;
					rotatedRect.size.width *= ksize_;
					rotatedRect.center += cv::Point2f(min[i].x, min[i].y);

					cv::Mat masked = cv::Mat::ones(roi.rows, roi.cols, roi.type())*255;
					roi.copyTo(masked, full_mask);

					enhanceComponent(masked, full_mask, size[i]);

					cv::Mat tmp = cv::Mat::zeros(masked.rows, masked.cols, masked.type());
					cv::inRange(masked, cv::Scalar(darkThreshold_), cv::Scalar(lightThreshold_), tmp);
					double mediumPixels = countNonZero(tmp);	// number of pixels in that are neither dark nor light
					//std::cout << "(mediumPixels, maskedPixels, ratio) = (" << mediumPixels << ", " << size[i] << "," << mediumPixels/size[i] << ")" << std::endl;
					if(mediumPixels/size[i] <= maxMediumRatio_) {
						// undo rotation
						ImageRotator rotator(cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(255));
						double rotAngle = (rotatedRect.size.width > rotatedRect.size.height) ? rotatedRect.angle : rotatedRect.angle + 90.0f;	// no need to resize the rotated rect, since the angle will be the same

						cv::Mat rotatedImage;
						rotator.rotate(masked, rotatedImage, rotAngle, 1.0);

						Roi candidate;
						candidate.image = rotatedImage;
						candidate.rotatedRect = rotatedRect;
						segments.push_back(candidate);

						//imshow("enhanced", rotatedImage);
						//cv::waitKey();
					}
				}
			}
		}
 	}
}

/**
 * When filter is ready, run the OpenCV function. According to amdahl's law any significant speedup has to optimize candidatesFromComponents() method which eats 2/3 of this filters cpu consumption and 3/4 of the overall barcode demonstrator.
 */
void dsa::BarcodeSegmenter::find(cv::Mat &image, std::vector<dsa::Roi> &segments) {
	// do some prefiltering
	cv::Mat tmp;
	prefilter(image, tmp);	// optimized, eats approx. 1/3 of filter performance

	// label
	std::vector<cv::Point> min;
	std::vector<cv::Point> max;
	std::vector<int> size;
	cv::Mat small_mask;

	// label the components
	label(tmp, small_mask, min, max, size);

	// identify and enhance components
	segmentsFromComponents(image, segments, small_mask, min, max, size); 	// TODO: needs optimization - eats approx 2/3 of filter performance and 3/4? of overall barcode demonstrator
}

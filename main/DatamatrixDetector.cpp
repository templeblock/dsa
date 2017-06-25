/*
 * DatamatrixDetector.cpp
 *
 *  Created on: 27.03.2012
 *      Author: jung
 */

#include "DatamatrixDetector.h"

dsa::DatamatrixDetector::DatamatrixDetector(const cv::Mat& mat) {
	image_ = mat;
}

dsa::DatamatrixDetector::~DatamatrixDetector() {
}

/**
 * Extract Datamatrix contents from the rectangle
 *
 * @param mat
 * @param threshold
 * @return
 */
const cv::Rect dsa::DatamatrixDetector::computeInnerDataMatrixRectangle(cv::Mat& mat, float threshold) {
	const UINT32 rowThreshold = threshold * mat.cols;
	const UINT32 colThreshold = threshold * mat.rows;

	UINT32 border[4] = { 0, 0, 0, 0 }; // represents clockwise: top, right, bottom, left line index
	UINT32 elements[4] = { 0, 0, 0, 0 }; // represents clockwise: number of white pixels in top, right, bottom, left line

	// top row (0 to rows/2)
	for(int i = 0; i < mat.rows/2; i++) {
		cv::Mat row = mat.row(i);
		const UINT32 white = countNonZero(row);
		const UINT32 black = row.cols - white;
		if(black > rowThreshold)
			elements[0] = white;
		else if(white >= elements[0]) {
			elements[0] = white;
			border[0] = i;
		}
	}
	// bottom row (rows to rows/2)
	for(int i = mat.rows-1; i > mat.rows/2; i--) {
		cv::Mat row = mat.row(i);
		const UINT32 white = countNonZero(row);
		const UINT32 black = row.cols - white;
		if(black > rowThreshold)
			elements[2] = white;
		else if(white >= elements[2]) {
			elements[2] = white;
			border[2] = i;
		}
	}
	// left column (0 to cols/2)
	for(int i = 0; i < mat.cols/2; i++) {
		cv::Mat col = mat.col(i);
		const UINT32 white = countNonZero(col);
		const UINT32 black = col.rows - white;
		if(black > colThreshold)
			elements[3] = 0;
		else if(white >= elements[3]) {
			elements[3] = white;
			border[3] = i;
		}
	}
	// right column (cols-1 to cols/2)
	for(int i = mat.cols-1; i > mat.cols/2; i--) {
		cv::Mat col = mat.col(i);
		const UINT32 white = countNonZero(col);
		const UINT32 black = col.rows - white;
		if(black > colThreshold)
			elements[1] = 0;
		else if(white >= elements[1]) {
			elements[1] = white;
			border[1] = i;
		}
	}

	return(cv::Rect(border[3], border[0], border[1]-border[3]+1, border[2]-border[0]+1));
}

/**
 * Determine the number of lines and columns where the number of black-white changes per line/column matches a typical pattern of a Datamatrix-Code
 *
 * @param mat: The image containing the alleged Datamatrix code
 *
 * @return the number of lines matching the criterion above
 */
int dsa::DatamatrixDetector::computeBlackWhiteChanges(cv::Mat& mat) {
	int success = 0;	// current number of consecutive lines with >= 4 edges per line
	int fails = 0;
	int hLines = 0; // maximum number of consecutive horizontal lines with >= 4 edges per line found so far
	int vLines = 0; // maximum number of consecutive vertical lines with >= 4 edges per line found so far
	const int failsUntilReset = mat.rows/10 + 1; // how long do we tolerate lines with < 4 edges until we reset

	// horizontal
	for(int j = 0; j < mat.rows; j++) {	// rows
		int previous = 0;
		int changes = 0;
		// possible enhancement: use getLine(...) and access Pixel there
		for(int i = 0; i < mat.cols; i++) { // columns in row
			int current = mat.at<UINT8>(j,i); // Mij
			// test if there is a black/white change
			if(abs(previous - current) == 255)
				changes++;
			previous = current;
		}
		if(changes > 3 && changes < 20) { // die 20 sind suboptimal, es sollten deutlich weniger sein
			// success
			success++;
			if(success > hLines)
				// update max consecutive edges found so far
				hLines = success;	// update maxNEdges
		} else if (success > 0) {
			// failure
			fails++;
			if(fails >= failsUntilReset) {
				// reset
				fails = 0;
				success = 0;
			}
		}
		//cout << "changes=" << changes << endl;
	}
	// vertical
	success = 0;
	fails = 0;
	for(int i = 0; i < mat.cols; i++) {	// columns
		int previous = 0;
		int changes = 0;
		for(int j = 0; j < mat.rows; j++) { // rows in column
			int current = mat.at<UINT8>(j,i); // Mij
			// test if there is a black/white change
			if(abs(previous - current) == 255)
				changes++;
			previous = current;
		}
		if(changes > 3 && changes < 20) { // die 20 sind suboptimal, es sollten deutlich weniger sein
			// success
			success++;
			if(success > vLines)
				// update max consecutive edges found so far
				vLines = success;	// update maxNEdges
		} else if (success > 0) {
			// failure
			fails++;
			if(fails >= failsUntilReset) {
				// reset
				fails = 0;
				success = 0;
			}
		}
		//cout << "changes=" << changes << endl;
	}

	return(hLines+vLines);
}

float dsa::DatamatrixDetector::computeAngle(Square& square) {
	cv::Point p[4] = square.p;
	float gradients[4];
	gradients[0] = computeGradient(p[0], p[1]);
	gradients[1] = computeGradient(p[1], p[2]);
	gradients[2] = computeGradient(p[2], p[3]);
	gradients[3] = computeGradient(p[3], p[0]);
	// find min and max gradient
	int iMin = 0;
	int iMax = 0;
	float minGrad = fabsf(gradients[0]);
	float maxGrad = fabsf(gradients[0]);
	for (int i = 1; i < 4; i++) {
		float current = fabsf(gradients[i]);
		if(current < minGrad) {
			minGrad = current;
			iMin = i;
		} else if(current > maxGrad) {
			maxGrad = current;
			iMax = i;
		}
	}
	// determine minimal angle for rotation
	float angle1 = atanf(gradients[iMin]) * 180.0f/PI;
	float angle2 = atanf(gradients[iMax]) * 180.0f/PI;
	float absAngle1 = fabsf(angle1);
	float absAngle2 = 90.0f - fabsf(angle2);
	float angle = 0.0f;
	if(absAngle1 < absAngle2)
		angle = angle1;
	else
		angle = (gradients[iMax] > 0.0f) ? -absAngle2 : absAngle2;

	return(angle);
}

const cv::Point dsa::DatamatrixDetector::DatamatrixDetector::find(SquareList &squareList) {
	cv::Point aoiCenter(-1, -1);
	UINT32 minPerimeter = UINT_MAX;
	for(squareList.s_it = squareList.squares.begin(); squareList.s_it != squareList.squares.end(); squareList.s_it++) {
		int minX = INT_MAX, minY = INT_MAX, maxX = -1, maxY = -1;
		cv::Point p[4] = (**squareList.s_it).p;
		for (int i = 0; i < 4; i++) {
			int x = p[i].x;
			int y = p[i].y;

			if(x > maxX) maxX = x;
			if(x < minX) minX = x;
			if(y > maxY) maxY = y;
			if(y < minY) minY = y;
		}
		// determine center of aoi in original frame
		cv::Point center = cv::Point((minX + maxX)/2, (minY + maxY)/2);
		// set ROI to rectangle around the located square
		cv::Mat subImage = image_(cv::Rect(minX, minY, maxX-minX+1, maxY-minY+1));
		cv::Mat aoi = subImage.clone();
		// create a mask with all black but the square
		cv::Mat mask = cv::Mat::zeros(aoi.rows, aoi.cols, CV_8UC1);

		// update point locations to match the aoiImage coordinate system
		for (int i = 0; i < 4; i++) {
			p[i].x -= minX;
			p[i].y -= minY;
		}
		maxX -= minX;
		maxY -= minY;
		minX = 0;
		minY = 0;

		cv::fillConvexPoly(mask, p, 4, cv::Scalar(255));
		// crop the polygon out of the image
		bitwise_and(aoi, mask, aoi);

		// check if average and standard deviation indicate 2dcode
		cv::Scalar mean = cv::Scalar(0);
		cv::Scalar std_dev = cv::Scalar(0);
		meanStdDev(aoi, mean, std_dev, mask);
		//cout << mean.val[0] << "," << std_dev.val[0] << endl;
		//if (mean.val[0] > 70 && mean.val[0] < 200 && std_dev.val[0] > 40) {
		if (std_dev[0] > 40) {
			// rotate aoiImage by minimal gradient where necessary
			const float angle = computeAngle(**squareList.s_it);

			// create copy of aoiImage
			cv::Mat aoi_dst = aoi.clone();
			cv::Mat mask_dst = mask.clone();
			if(fabs(angle) > minRotAngle) {
				cv::Point2f center(maxX*0.5f, maxY*0.5f);
				cv::Mat rotationMatrix = getRotationMatrix2D(center, angle, 1.0);
				// rotate the image by warpAffine taking rotationMatrix
				cv::warpAffine(aoi, aoi_dst, rotationMatrix, aoi.size(), cv::INTER_CUBIC);
				cv::warpAffine(mask, mask_dst, rotationMatrix, mask.size(), cv::INTER_NEAREST);

				// crop black border
				cv::Point ptick[4];
				// rotate all corners of the original square by the same angle
				float radAngle = angle*PI/180;
				ptick[0] = rotatePoint(p[0], center, radAngle);
				ptick[1] = rotatePoint(p[1], center, radAngle);
				ptick[2] = rotatePoint(p[2], center, radAngle);
				ptick[3] = rotatePoint(p[3], center, radAngle);

				// determine new corners
				int rMinX = INT_MAX, rMinY = INT_MAX, rMaxX = -1, rMaxY = -1;
				for (int i = 0; i < 4; i++) {
					int x = ptick[i].x;
					int y = ptick[i].y;
					if(x > rMaxX)
						rMaxX = (x < maxX) ? x : maxX;
					if(x < rMinX)
						rMinX = (x > minX) ? x : minX;
					if(y > rMaxY)
						rMaxY = (y < maxY) ? y : maxY;
					if(y < rMinY)
						rMinY = (y > minY) ? y : minY;
				}
				// update min and max values to smaller image
				minX = rMinX;
				minY = rMinY;
				maxX = rMaxX;
				maxY = rMaxY;
				// crop around (rMinX,rMinY)->(rMaxX,rMaxY) + cut an additional 10% margin (a little piece of the white or black border)
				int margin = (rMaxX-rMinX+rMaxY-rMinY)/20;
				int width = std::min(aoi_dst.cols-rMinX-margin, rMaxX-rMinX+1-2*margin);
				int height = std::min(aoi_dst.rows-rMinY-margin, rMaxY-rMinY+1-2*margin);
				cv::Rect rect(rMinX+margin, rMinY+margin, width, height);
				aoi_dst = cv::Mat(aoi_dst, rect);
				mask_dst = cv::Mat(mask_dst, rect);
				//imshow("ROI2", aoi_dst);
			}

			// sharpen image
			cv::GaussianBlur(aoi_dst, aoi, cv::Size(0, 0), 3.0);
			addWeighted(aoi_dst, 1.5, aoi, -0.5, 0.0, aoi_dst);
			// remove noise
			cv::resize(aoi_dst, aoi_dst, cv::Size(0, 0), 2.0, 2.0, cv::INTER_CUBIC);
			cv::resize(aoi_dst, aoi_dst, cv::Size(0, 0), 0.5, 0.5, cv::INTER_NEAREST);

			// copy image to do adaptive threshold
			UINT32 filterWindowSize = (aoi_dst.rows + aoi_dst.cols)/6;
			if(filterWindowSize%2 == 0)
				filterWindowSize += 1;
			cv::Mat threshMat = aoi_dst.clone();
			cv::adaptiveThreshold(threshMat, threshMat, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, filterWindowSize , 10);

			// crop datamatrix code
			// crop around rectangle defined by (leftcropcol, topcroprow), (rightcropcol, bottomcroprow)
			cv::Rect rect = computeInnerDataMatrixRectangle(threshMat, 0.9f);
			cv::Mat innerRectangle(aoi_dst, rect);
			//rect = computeInnerDataMatrixRectangle(innerRectangle, 1.0f);
			//innerRectangle = Mat(innerRectangle, rect);

			filterWindowSize = (innerRectangle.rows + innerRectangle.cols)/6; // a third of image (height+width)/2
			if(filterWindowSize%2 == 0)
				filterWindowSize += 1;
			cv::adaptiveThreshold(innerRectangle, innerRectangle, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, filterWindowSize, 10);

			//imshow("ROI", innerRectangle);
			//imshow("ROI2", threshMat);
			//waitKey(0);
			//resize(innerRectangle, innerRectangle, Size(640, 480), 2, 2, INTER_CUBIC);

			const int bwChanges = computeBlackWhiteChanges(innerRectangle);
			float fractionOf2DCode = (float)bwChanges/(innerRectangle.rows+innerRectangle.cols);
			UINT32 perimeter = (UINT32)computeSquarePerimeter(p);
			//UINT32 perimeterApprox = 2 * mask.cols + 2 * mask.rows; // approximation: perimeter of the bounding box
			if(fractionOf2DCode > 0.5f && perimeter < minPerimeter) {
				minPerimeter = perimeter;
				aoiCenter.x = center.x;
				aoiCenter.y = center.y;
			}
			//cout << "Anteil 2D-code " << fractionOf2DCode << " (" << bwChanges << ", " << innerRectangle.rows+innerRectangle.cols << ")"  << endl;
		}

		// cleanup
		delete *(squareList.s_it);
	}
	// return center point of the last marker found so far.
	return(aoiCenter);
}

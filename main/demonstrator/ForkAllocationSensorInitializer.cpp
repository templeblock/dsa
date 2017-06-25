/*
 * ForkAllocationSensorInitializer.cpp
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#include "ForkAllocationSensorInitializer.h"

dsa::ForkAllocationSensorInitializer::ForkAllocationSensorInitializer(VideoHub* dataHub, std::string &configFile) : dsa::Sensorfunction(SensorType::ForkOccupied) {
	configFile_ = configFile;
	useConfig_ = readFromXML(configFile_);
	skipDetermineBinary_ = false;
	hub_ = dataHub;

	init();
}

dsa::ForkAllocationSensorInitializer::ForkAllocationSensorInitializer() : dsa::Sensorfunction(SensorType::ForkOccupied) {
	useConfig_ = false;
	skipDetermineBinary_ = true;
	hub_ = 0;

	init();
}

dsa::ForkAllocationSensorInitializer::ForkAllocationSensorInitializer(std::string &configFile) : dsa::Sensorfunction(SensorType::ForkOccupied) {
	configFile_ = configFile;
	useConfig_ = true;
	skipDetermineBinary_ = true;

	init();
}

dsa::ForkAllocationSensorInitializer::~ForkAllocationSensorInitializer() {
	delete filterChain_;
	for(size_t i = 0; i < maskedPixelsPerLine_.size(); i++)
		delete maskedPixelsPerLine_[i];
}

bool dsa::ForkAllocationSensorInitializer::calibrate() {
	return(true);
}

/**
 * Helper function to initialize for phase1
 */
void dsa::ForkAllocationSensorInitializer::determineBinaryImage() {
	filterChain_->apply();	// apply all filters in chain
	cv::waitKey(1);
	// if we used an image in ForkBackgroundFilter, there was a significantly change in the image. we need approx 100? such changes, so drive along until those are reached
	std::list<Filter*>::iterator it = filterChain_->getChain()->begin();
	it++;
	it++;
	it++;
	if(((ForkBackgroundFilter*)(*it))->usedLastImage())
		runs_++;
}

/**
 * Helper function to initialize for phase1
 */
void dsa::ForkAllocationSensorInitializer::determineBinaryImage(cv::Mat *input) {
	((ImageFilter*)filterChain_->getChain()->front())->src = input;
	filterChain_->apply();	// apply all filters in chain
	// if we used an image in ForkBackgroundFilter, there was a significantly change in the image. we need approx 100? such changes, so drive along until those are reached
	std::list<Filter*>::iterator it = filterChain_->getChain()->begin();
	it++;
	it++;
	it++;
	if(((ForkBackgroundFilter*)(*it))->usedLastImage())
		runs_++;
}

cv::Mat dsa::ForkAllocationSensorInitializer::getBinaryImage() {
	return(binaryImage_);
}

/**
 * Helper function to initialize
 */
void dsa::ForkAllocationSensorInitializer::init() {
	runs_ = 0;
	GrabImageFilter		*filter0 = new GrabImageFilter(*hub_);
	RotationFilter	*filter1 = new RotationFilter(0, cv::Size(816, 686));
	CvtColorFilter *filter2 = new CvtColorFilter(CV_RGB2HSV);
	//filter2->setActive(false);
	ForkBackgroundFilter *filter3 = new ForkBackgroundFilter(true);
	ImageOutputFilter	*filter4 = new ImageOutputFilter("BinaryImage");	// show video
	//filter4->setActive(false);
	WaitFilter			*filter5 = new WaitFilter(15.f);	// wait assuming 10fps

	// set relations (input parameters of each filter)
	filter1->src = &(filter0->dst); // filter1 is successor of filter0
	filter2->src = &(filter1->dst); // filter2 is successor of filter1
	filter3->src = &(filter2->dst); // filter3 is successor of filter2
	filter4->src = &(filter3->dst); // filter4 is successor of filter3
	filter5->src = &(filter4->dst); // filter5 is successor of filter4

	// list of the filters for phase 1
	std::list<Filter*> *filterList = new std::list<Filter*>();
	// add filters to list
	filterList->push_back(filter0);
	filterList->push_back(filter1);
	filterList->push_back(filter2);
	filterList->push_back(filter3);
	filterList->push_back(filter4);
	filterList->push_back(filter5);

	// create filterchain for phase 1
	filterChain_ = new FilterChain(filterList, true);
}

void dsa::ForkAllocationSensorInitializer::findForks() {
	// continue working on the final image
	binaryImage_ = 1-binaryImage_; // invert to make fork white (frame around the image is considered black by the polygon approximation algorithm)

	const double maxWidthRatio = 1.0/3.0;	// max value for width/length
	const double minWidthRatio = 0.05; // min value for width/length
	const double forkFOVratioY = 1.0/3.0;	// see  above
	const double minForkPerimeter = 2 * forkFOVratioY * std::max(binaryImage_.cols, binaryImage_.rows);	// 1/3 der längeren Seite
	const double minForkLengthRatio = 0.5;
	const double minForkWidthRatio = 0.5;
	const double maxForkAngleDifference = 40.0;	// maximal difference in angle between the forks
	const double forkBorderMarginRatio = 0.05;	// how far away from the border of the image may the forks start (ratio of the length of the border involved)
	const int maxYTop = (int)(binaryImage_.rows * forkBorderMarginRatio); // minimum Y value to be near the bottom border
	const int maxXLeft = (int)(binaryImage_.cols * forkBorderMarginRatio); // maximum X value to be near the left border
	const int minYBottom = (int)(binaryImage_.rows * (1 - forkBorderMarginRatio)); // minimum Y value to be near the bottom border
	const int minXRight = (int)(binaryImage_.cols * (1 - forkBorderMarginRatio)); // minimum X value to be near the right border

	std::vector<std::vector<cv::Point> > contours;
	std::vector<std::vector<cv::Point> > polygons;
	std::vector<std::vector<char> > adjacentLines;
	std::vector<double> lengths;
	std::vector<double> widths;
	std::vector<double> angles;
	cv::findContours(binaryImage_, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for(size_t i = 0; i < contours.size(); i++) {
		cv::Mat polygon(contours[i]);
		double perimeter = arcLength(polygon, true);
		if(perimeter >= minForkPerimeter) {	// filter small polygons to speed up process (saves computation time). The real length check is performed below.
			std::cout << "Accepted polygon Gabelkandidat" << i << " because perimeter " << perimeter << " was >= minForkPerimeter " << minForkPerimeter << std::endl;
			std::vector<cv::Point> approxpolygon, hull;
			// approximate contour by polygon
			cv::approxPolyDP(polygon, approxpolygon, perimeter*0.01, true);
			// modify polygon to its convex hull
			cv::Mat convexPolygon(approxpolygon);
			cv::convexHull(convexPolygon, hull); // get back edges that were bitten off by lighting conditions

			double perimeter = arcLength(hull, true); // check length again for convex hull, as the arclength of the convex hull is shorter but describes our fork more precisely
			if(perimeter >= minForkPerimeter) {	// filter small polygones by perimeter
				std::cout << "Accepted polygon Gabelkandidat" << i << " because convex hull perimeter " << perimeter << " was >= minForkPerimeter " << minForkPerimeter << std::endl;
				// approximate length and width of the polygon by length and width of the bounding rotated rectangle with the minimal area
				double area = contourArea(hull);
				cv::Rect forkBoundingRect = boundingRect(hull);
				double length = sqrt(forkBoundingRect.width*forkBoundingRect.width + forkBoundingRect.height*forkBoundingRect.height);
				double width = area/length;
				// debug the candidates
				cv::Mat output = cv::Mat::ones(binaryImage_.rows, binaryImage_.cols, CV_8UC1)*255; // mask is initially 255 everywhere
				cv::fillConvexPoly(output, hull, cv::Scalar(0));
				cv::rectangle(output, forkBoundingRect, cv::Scalar(127));
				std::string filename = "Gabelkandidat";
				filename.append(intToString(i));
				filename.append(".png");
				imwrite(filename, output);
    			//cv::waitKey(0);
				double widthByLength = width/length;
				if(widthByLength < maxWidthRatio && widthByLength > minWidthRatio) {
					// check if two consecutive points in the polygon are near the image border
					std::set<char> borders;
					for(size_t j = 0; j < hull.size(); j++) {
						cv::Point current = hull[j];
						cv::Point next = hull[(j+1)%hull.size()];
						// one line cannot be near top/bottom and left/right at the same time, but all other combinations are possible
						if(current.y <= maxYTop && next.y <= maxYTop)	// near top
							borders.insert(0);
						else if(current.y >= minYBottom && next.y >= minYBottom)	// near bottom
							borders.insert(2);
						if((current.x <= maxXLeft && next.x <= maxXLeft)) // near left
							borders.insert(1);
						else if((current.x >= minXRight && next.x >= minXRight)) // near right
							borders.insert(3);
					}
					if(borders.size() == 1 || borders.size() == 2) { // we are close to 1 or 2 borders
						cv::RotatedRect forkMinAreaRect = minAreaRect(hull); // we want to know the approximate angle
						angles.push_back(forkMinAreaRect.angle);
						polygons.push_back(hull);
						lengths.push_back(length);
						widths.push_back(width);
						std::vector<char> temp(borders.begin(), borders.end());
						adjacentLines.push_back(temp);

					} else  // else not a fork
						std::cout << "Threw polygon Gabelkandidat" << i << " out, because it is not close to 1 or 2 borders." <<  std::endl;
				} else
					std::cout << "Threw polygon Gabelkandidat" << i << " out, because width/length " << widthByLength << " was not in acceptable range ]" << minWidthRatio << ";" << maxWidthRatio << "["<<  std::endl;
			}
		}
	}

	// final evaluation: forks must be roughly equally long and near the same or a neighbor border
	// we take the two remaining polygons that match our criteria best or restart if no suitable pair can be found
	bool found = false;
	if(polygons.size() >= 2) { // more than two candidates => choose pair with best lengthratio (would also cover 2 polygon case, so remove the above when tested
		double bestLengthRatio = 0;
		for(size_t i = 0; i < polygons.size(); i++) {	// brute force all combinations as we only deal with really few candidates here.
			for(size_t j = i+1; j < polygons.size(); j++) {
				double lengthRatio = (lengths[i] < lengths[j]) ? lengths[i]/lengths[j] : lengths[j]/lengths[i];
				double widthRatio = (widths[i] < widths[j]) ? widths[i]/widths[j] : widths[j]/widths[i];
				double angleDiff = fabs(angles[i] >= angles[j] ? angles[i] - angles[j] : angles[j] - angles[i]);
				if(angleDiff <= maxForkAngleDifference && lengthRatio >= minForkLengthRatio && lengthRatio > bestLengthRatio && widthRatio > minForkWidthRatio) { // angle and length similar, but only examine combos with better lengthRatio
					// valid pair is near same border or at least not near opposing borders
					bool opposingBorder = false;
					bool sameBorder = false;
					for(size_t k = 0; k < adjacentLines[i].size(); k++) {
						for(size_t l = 0; l < adjacentLines[j].size(); l++) {
							if(adjacentLines[i][k] == adjacentLines[j][l]) {
								sameBorder = true;	// OK
								break;
							} else if((adjacentLines[i][k]+adjacentLines[j][l])%2 == 0) // Summe der Nummern gegenüberliegender Seiten ist gerade => gegenüberliegende Seiten
								opposingBorder = true;
						}
						if(sameBorder)
							break;
					}
					if(sameBorder || !opposingBorder) {	// matching pair (liegen an gleicher Seite an oder zumindest nicht an gegenüberliegenden)
						found = true;
						bestLengthRatio = lengthRatio;	// update best lengthRatio
						forks_.clear();					// remove previous best candidates if any
						forks_.push_back(polygons[i]);	// add new best candidates
						forks_.push_back(polygons[j]);
					}
				}
			}
		}
	}

	// part 3: make use of the found fork mask and show video again, this time only the areas we want to see.
	if(found) {	// add the found forks to mask
		// determine ROIs
		for(size_t i = 0; i < forks_.size(); i++) {
			cv::Rect roi = boundingRect(cv::Mat(forks_[i]));
			cv::Mat forkMask = cv::Mat::zeros(roi.height, roi.width, CV_8UC1); // mask is initially 0 everywhere
			cv::Mat forkAverage(forkReferenceImage_, roi);
			cv::Point offset = cv::Point(roi.x, roi.y);

			// update fork to match the coordinates in ROI
			for(size_t j = 0; j < forks_[i].size(); j++)
				forks_[i][j] -= offset;

			cv::fillConvexPoly(forkMask, forks_[i], cv::Scalar(255));

			// determine the reference image (average over a couple of frames)
			cv::Mat forkReference;
			bitwise_and(forkAverage, forkMask, forkReference);

			// save the number of masked pixels per line for normalization
			std::vector<size_t> *maskedPixelsPerLine = new std::vector<size_t>();
			for(size_t i = 0; i < forkMask.rows; i++) {
				maskedPixelsPerLine->push_back(sum(forkMask.row(i)).val[0]/255);
			}

			// fork histogram
			cv::Mat hist;
		    const int histSize = 16;
			const float histRange[] = {0.0f, 255.0f};
		    const float* phistRange = histRange;
			cv::calcHist(&forkReference, 1, 0, forkMask, hist, 1, &histSize, &phistRange);
			normalize(hist, hist, 0, 255, CV_MINMAX);

			rois_.push_back(roi);
			forkMasks_.push_back(forkMask.clone());
			maskedPixelsPerLine_.push_back(maskedPixelsPerLine);
			forkReferences_.push_back(forkReference.clone());
			forkHistograms_.push_back(hist);
		}

		// write determined info to xml file
		writeToXML(configFile_);
		// only keep the roi, clear the (larger) rest
		binaryImage_.release();
		forkReferenceImage_.release();
	} else {
		reset();
	}
}

void dsa::ForkAllocationSensorInitializer::run() {
	if(!useConfig_) { // if nothing is found after 10s, start over
		if(runs_ > 50)
			reset();
		// apply filterchain for a few seconds (~10-20) while forklift ist moving and not allocated
		if(!skipDetermineBinary_) {
			while(runs_ < 50) // loop for n frames
				determineBinaryImage();
		}

		// save the created images for later use
		std::list<Filter*>::iterator it = filterChain_->getChain()->begin();
		it++;
		it++;
		it++;
		binaryImage_ = ((ForkBackgroundFilter*)(*it))->dst.clone();
		forkReferenceImage_ = ((ForkBackgroundFilter*)(*it))->getAverage().clone();

		findForks();
	}
}

bool dsa::ForkAllocationSensorInitializer::isReady() {
	return(forkReferences_.size() == 2);
}

void dsa::ForkAllocationSensorInitializer::reset() {
	runs_ = 0;
	// reset ForkBackgroundFilter
	std::list<Filter*>::iterator it = filterChain_->getChain()->begin();
	it++;
	it++;
	it++;
	((ForkBackgroundFilter*)(*it))->reset();

	// delete all local data in initializer
	binaryImage_.release();
	forkReferenceImage_.release();
	rois_.clear();
	forks_.clear();
	forkMasks_.clear();
	forkReferences_.clear();
	forkHistograms_.clear();
	for(size_t i = 0; i < maskedPixelsPerLine_.size(); i++) {
		delete maskedPixelsPerLine_[i];
	}
	maskedPixelsPerLine_.clear();
}

void dsa::ForkAllocationSensorInitializer::setConfig(std::string &configFile) {
	configFile_ = configFile;
}

std::vector<cv::Rect>& dsa::ForkAllocationSensorInitializer::getROIs() {
	return(rois_);
}

std::vector<std::vector<cv::Point> >& dsa::ForkAllocationSensorInitializer::getForks() {
	return(forks_);
}

std::vector<cv::Mat>& dsa::ForkAllocationSensorInitializer::getMasks() {
	return(forkMasks_);
}

std::vector<cv::Mat>& dsa::ForkAllocationSensorInitializer::getReferences() {
	return(forkReferences_);
}

std::vector<cv::Mat>& dsa::ForkAllocationSensorInitializer::getHistograms() {
	return(forkHistograms_);
}

std::vector<std::vector<size_t> *>& dsa::ForkAllocationSensorInitializer::getMaskedPixelsPerLine() {
	return(maskedPixelsPerLine_);
}

bool dsa::ForkAllocationSensorInitializer::writeToXML(const std::string &filename) const {
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	bool success = fs.isOpened();
	if(success && forks_.size() == 2) {
		std::vector<cv::Point> roiHelper1 = std::vector<cv::Point>();
		roiHelper1.push_back(rois_[0].tl());
		roiHelper1.push_back(rois_[0].br());
		std::vector<cv::Point> roiHelper2 = std::vector<cv::Point>();
		roiHelper2.push_back(rois_[1].tl());
		roiHelper2.push_back(rois_[1].br());

		fs << "roi1" << roiHelper1;
		fs << "roi2" << roiHelper2;
		fs << "fork1" << forks_[0];
		fs << "fork2" << forks_[1];
		fs << "forkReference1" << forkReferences_[0];
		fs << "forkReference2" << forkReferences_[1];
		fs << "forkHistogram1" << forkHistograms_[0];
		fs << "forkHistogram2" << forkHistograms_[1];
	}
	fs.release();

	return(success);
}

bool dsa::ForkAllocationSensorInitializer::readFromXML(const std::string &filename) {
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	bool success = fs.isOpened();
	if(success) {
		std::vector<cv::Point> roiHelper1 = std::vector<cv::Point>();
		std::vector<cv::Point> roiHelper2 = std::vector<cv::Point>();
		std::vector<cv::Point> fork1 = std::vector<cv::Point>();
		std::vector<cv::Point> fork2 = std::vector<cv::Point>();
		cv::Mat reference1 = cv::Mat();
		cv::Mat reference2 = cv::Mat();
		cv::Mat histogram1 = cv::Mat();
		cv::Mat histogram2 = cv::Mat();

		fs["roi1"] >> roiHelper1;
		fs["roi2"] >> roiHelper2;
		fs["fork1"] >> fork1;
		fs["fork2"] >> fork2;
		fs["forkReference1"] >> reference1;
		fs["forkReference2"] >> reference2;
		fs["forkHistogram1"] >> histogram1;
		fs["forkHistogram2"] >> histogram2;

		// create rectangle from vector of the points. first point represents upper left corner, second point represents width and height
		cv::Rect roi1 = cv::Rect(roiHelper1[0], roiHelper1[1]);
		cv::Rect roi2 = cv::Rect(roiHelper2[0], roiHelper2[1]);

		rois_.push_back(roi1);
		rois_.push_back(roi2);
		forks_.push_back(fork1);
		forks_.push_back(fork2);
		forkReferences_.push_back(reference1);
		forkReferences_.push_back(reference2);
		forkHistograms_.push_back(histogram1);
		forkHistograms_.push_back(histogram2);

		// draw forkMasks and calculate corresponding maskedPixelsPerLines
		// forkMask1
		std::vector<size_t> *maskedPixelsPerLine = new std::vector<size_t>();
		cv::Mat forkMask = cv::Mat::zeros(roi1.height, roi1.width, CV_8UC1); // mask is initially 0 everywhere
		cv::fillConvexPoly(forkMask, fork1, cv::Scalar(255));
		forkMasks_.push_back(forkMask.clone());
		for(size_t i = 0; i < forkMask.rows; i++)
			maskedPixelsPerLine->push_back(sum(forkMask.row(i)).val[0]/255);
		maskedPixelsPerLine_.push_back(maskedPixelsPerLine);

		// forkMask2
		maskedPixelsPerLine = new std::vector<size_t>();
		forkMask = cv::Mat::zeros(roi2.height, roi2.width, CV_8UC1); // mask is initially 0 everywhere
		cv::fillConvexPoly(forkMask, fork2, cv::Scalar(255));
		forkMasks_.push_back(forkMask.clone());
		for(size_t i = 0; i < forkMask.rows; i++)
			maskedPixelsPerLine->push_back(sum(forkMask.row(i)).val[0]/255);
		maskedPixelsPerLine_.push_back(maskedPixelsPerLine);
	}
	fs.release();

	return(success);
}

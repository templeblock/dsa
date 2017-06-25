/*
 * DemonstratorTF.cpp
 *
 *  Created on: 19.04.2012
 *      Author: Jung
 */

#include "DemonstratorTF.h"

dsa::DemonstratorTF::DemonstratorTF(const UINT32 camera, const UINT32 predictionRadius) {
	this->predictionRadius = predictionRadius;
	capture = cv::VideoCapture(camera);
}

dsa::DemonstratorTF::~DemonstratorTF() {
}

int dsa::DemonstratorTF::run() {
	if(!capture.isOpened()) {
		std::cout << "Failed to open file!" << std::endl;
		return(-1);
	} else {
		cv::namedWindow("Video", CV_WINDOW_AUTOSIZE);

		bool stop = false;
		int hits = 0;
		int misses = 0;
		VectorPredictor<cv::Point> pred = VectorPredictor<cv::Point>();
		//ExpFilterPredictor<cv::Point> pred = ExpFilterPredictor<cv::Point>(0.9f);
		//StaticPredictor<cv::Point> pred = StaticPredictor<cv::Point>();
		while(!stop) {
			clock_t c_start = clock();
			cv::Mat frame;

			bool grabSuccess = capture.grab();
			bool retrieveSuccess = capture.retrieve(frame, 0);	// retrieve only gray channel
			if(!grabSuccess || !retrieveSuccess || !frame.data) {
				// retrieving failed
				std::cout << "Query failed!" << std::endl;
				break;
			}

			// the estimated position
			bool predicted = false;
			cv::Point est;
			if(pred.isInitialized()) {
				predicted = true;
				est = pred.next();
				cv::Point e1(est.x-1, est.y-1);
				cv::Point e2(est.x+1, est.y-1);
				cv::Point e3(est.x+1, est.y+1);
				cv::Point e4(est.x-1, est.y+1);
				drawSquare(frame, e1, e2, e3, e4, cv::Scalar(0, 0, 255));
				cv::circle(frame, est, predictionRadius, cv::Scalar(0, 0, 255), 2);
			}

			cv::Mat filteredImage(frame.rows, frame.cols, CV_8UC1);
			cv::cvtColor(frame, filteredImage, CV_RGB2GRAY);
			cv::equalizeHist(filteredImage, filteredImage);

			cv::Mat outputImage = filteredImage.clone();
			cv::Mat resizedImage = filteredImage.clone();
			// remove noise
			cv::resize(resizedImage, resizedImage, cv::Size(0, 0), 0.25, 0.25, cv::INTER_LINEAR);
			cv::resize(resizedImage, resizedImage, cv::Size(0, 0), 4.0, 4.0, cv::INTER_LINEAR);

			cv::adaptiveThreshold(resizedImage, resizedImage, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 51, 10);
			cv::dilate(resizedImage, resizedImage, cv::Mat());

			SquareDetector squareDetector(resizedImage);
			SquareList* squareList = squareDetector.find();
			if(! squareList->squares.empty()) {
				// evaluate
				DatamatrixDetector datamatrixDetector(outputImage);
				cv::Point center = datamatrixDetector.find(*squareList);
				if(center.x > 0 && center.y > 0) {
					cv::Point p1(center.x-1, center.y-1);
					cv::Point p2(center.x+1, center.y-1);
					cv::Point p3(center.x+1, center.y+1);
					cv::Point p4(center.x-1, center.y+1);
					drawSquare(frame, p1, p2, p3, p4);
					if(predicted) {
						float dist = computeLength(center, est);
						if(dist <= predictionRadius) { // correctly predicted => hit
							hits++;
							pred.hit();
						} else { // predicted, but actual center not within radius => miss
							misses++;
							pred.missed();
						}
					} else { // not predicted but found => miss
						misses++;
						pred.missed();
					}
					pred.feed(center); // update predictor with new value
				} else {
					if(predicted) { // predicted but none found => miss
						misses++;
						pred.missed();
					}
				}
			} else {
				if(predicted) { // predicted but not even a square found, let alone a marker => miss
					misses++;
					pred.missed();
				}
			}
			delete squareList;

			// mark center of the recognized marker
			cv::imshow("Video", frame);
			// wait in unlikely case we process video too fast
			int nTicks = (int)(clock() - c_start);
			int wait = (msPerFrame - (int)(msPerTick * nTicks));
			stop = (wait > 0) ? (cv::waitKey(wait) == 'q') : false;
		}

		cv::destroyAllWindows();
		// print overall hit ratio for this run
		std::cout << (float)hits/(hits+misses) << std::endl;
		return(0);
	}
}

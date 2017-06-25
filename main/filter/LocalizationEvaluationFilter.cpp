/*
 * LocalizationEvaluationFilter.cpp
 *
 * Generate file with localization evaluation data
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "LocalizationEvaluationFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::LocalizationEvaluationFilter::LocalizationEvaluationFilter(std::string &filename) {
	toggle_ = false;
	toggleCount_ = 0;
	outfile_.open(filename.c_str(), std::ios_base::out);
	worker_ = new WaitForInputWorker();
	listenForInputThread_ = new boost::thread(&WaitForInputWorker::run, &(*worker_));
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::LocalizationEvaluationFilter::~LocalizationEvaluationFilter() {
	outfile_.close();
	// cleanup thread environment
	worker_->finish();
	listenForInputThread_->join();
	delete worker_;
	delete listenForInputThread_;
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::LocalizationEvaluationFilter::init() {
	//outfile_ << "timestamp;x1;x2;x3;a1;a2;a3;" << std::endl;
	outfile_ << "timestamp;x1;x2;a3;fh;fx;fy;fr;keyset;locV;fhV;frV" << std::endl;
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::LocalizationEvaluationFilter::evaluate() {
	if(camPosition->at<float>(0,0) != -FLT_MAX) {
		// Abschnitte markieren mit Tastendruck
		std::string keystatus = std::string("");
		if(worker_->isFinished()) {
			exit(0);
		}
		if(worker_->wasKeyPressed()) {
			keystatus += capital_alphabet[toggleCount_];
			toggle_ = true;
		} else if(toggle_ && !worker_->wasKeyPressed()) {
			toggleCount_ = (toggleCount_+1)%26;
			toggle_ = false;
		}

		std::string line = std::string(intToString(*timestamp));
		line.append(";");
		line.append(doubleToString(camPosition->at<float>(0, 0)));
		line.append(";");
		line.append(doubleToString(camPosition->at<float>(0, 1)));
		line.append(";");
		/*line.append(doubleToString(camPosition->at<float>(0, 2)));
		line.append(";");
		line.append(doubleToString(180.0f*camRotation->at<float>(0, 0)/PI));
		line.append(";");
		line.append(doubleToString(180.0f*camRotation->at<float>(0, 1)/PI));
		line.append(";");*/
		line.append(doubleToString(180.0f*camRotation->at<float>(0, 2)/PI));
		line.append(";");
		line.append(doubleToString(*forkHeight));
		line.append(";");
		line.append(doubleToString((double)(forkEndPosition->x)));
		line.append(";");
		line.append(doubleToString((double)(forkEndPosition->y)));
		line.append(";");
		line.append(doubleToString(180.0f*(*forkRotation)/PI));
		line.append(";");
		line.append(keystatus);
		line.append(";");
		line.append(intToString(*haveValue2));
		line.append(";");
		line.append(intToString(*haveValue1));
		line.append(";");
		line.append(intToString(*haveValue3));
		line.append(";");
		outfile_ << line << std::endl;
	}
}

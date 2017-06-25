/*
 * EvaluationLogger.cpp
 *
 * Generate file with localization evaluation data
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "EvaluationLogger.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::EvaluationLogger::EvaluationLogger(std::string &filename, WaitForInputWorker *worker) {
	lastTimestamp_ = 0;
	toggle_ = false;
	toggleCount_ = 0;
	outfile_.open(filename.c_str(), std::ios_base::out);
	worker_ = worker;
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::EvaluationLogger::~EvaluationLogger() {
	outfile_.close();
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::EvaluationLogger::init() {
	outfile_ << "timestamp;x1;x2;a3;fh;fr;keyset;repeat" << std::endl;
}

/**
 * Log a new value
 */
void dsa::EvaluationLogger::log(LocalizationResult *localizationResult, ForkHeightResult *forkHeightResult) {
	if(localizationResult->isValid || forkHeightResult->isValid) {
		// Abschnitte markieren mit Tastendruck
		std::string keystatus = std::string("");
		if(!worker_->isFinished()) {
			if(worker_->wasKeyPressed()) {
				keystatus += capital_alphabet[toggleCount_];
				toggle_ = true;
			} else if(toggle_ && !worker_->wasKeyPressed()) {
				toggleCount_ = (toggleCount_+1)%26;
				toggle_ = false;
			}
		}
		int timestamp = localizationResult->isValid ? localizationResult->timestamp : forkHeightResult->timestamp;

		std::string repeat = lastTimestamp_ == timestamp ? "1" : "0";
		lastTimestamp_ = timestamp;

		std::string line = std::string(intToString(timestamp));
		line.append(";");
		line.append(doubleToString(localizationResult->t[0]));
		line.append(";");
		line.append(doubleToString(localizationResult->t[1]));
		line.append(";");
		line.append(doubleToString(180.f/PI*localizationResult->r[2]));
		line.append(";");
		if(forkHeightResult->isValid) {
			line.append(doubleToString(forkHeightResult->h));
			line.append(";");
			line.append(doubleToString(180.f/PI*forkHeightResult->r));
			line.append(";");
			line.append(doubleToString(forkHeightResult->endX));
			line.append(";");
			line.append(doubleToString(forkHeightResult->endZ));
		} else {
			line.append(";;;");
		}
		line.append(";");
		line.append(keystatus);
		line.append(";");
		line.append(repeat);
		line.append(";");
		outfile_ << line << std::endl;
	}
}

/**
 * Log a new barcode event
 * @param barcodeResult
 */
void dsa::EvaluationLogger::log(BarcodeResult *barcodeResult) {
	std::string keystatus = std::string("");
	if(!worker_->isFinished()) {
		if(worker_->wasKeyPressed()) {
			keystatus += capital_alphabet[toggleCount_];
			toggle_ = true;
		} else if(toggle_ && !worker_->wasKeyPressed()) {
			toggleCount_ = (toggleCount_+1)%26;
			toggle_ = false;
		}
	}

	if(barcodeResult->isValid) {
		for(size_t i = 0; i < barcodeResult->barcodeEvents.size(); i++) {
			std::string line = std::string(intToString(barcodeResult->barcodeEvents[i].timestamp));
			line.append(";");
			line.append(intToString(barcodeResult->barcodeEvents[i].barcode.format));
			line.append(";");
			line.append(barcodeResult->barcodeEvents[i].barcode.contents);
			line.append(";");
			line.append(keystatus);
			line.append(";");
			outfile_ << line << std::endl;
			std::cout << "read: " << intToString(barcodeResult->barcodeEvents[i].barcode.format) << "," << barcodeResult->barcodeEvents[i].barcode.contents << std::endl;
		}
	} else if(keystatus.length() > 0) {
		std::string line = std::string(";;;");
		line.append(keystatus);
		line.append(";");
		outfile_ << line << std::endl;
	}
}

/**
 * Log a new ForkstateEvent
 * @param forkInfo
 */
void dsa::EvaluationLogger::log(ForkInfo* forkInfo) {
	std::string keystatus = std::string("");
	if(!worker_->isFinished()) {
		if(worker_->wasKeyPressed()) {
			keystatus += capital_alphabet[toggleCount_];
			toggle_ = true;
		} else if(toggle_ && !worker_->wasKeyPressed()) {
			toggleCount_ = (toggleCount_+1)%26;
			toggle_ = false;
		}
	}

	std::string line = std::string(intToString(currentTimeOfDayMilliseconds()));
	line.append(";");
	line.append(intToString((int)forkInfo->getLeft()));
	line.append(";");
	line.append(intToString((int)forkInfo->getRight()));
	line.append(";");
	line.append(intToString((int)forkInfo->getState()));
	line.append(";");
	line.append(keystatus);
	line.append(";");
	outfile_ << line << std::endl;
}


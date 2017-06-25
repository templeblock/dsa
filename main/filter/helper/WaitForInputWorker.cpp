/*
 * WaitForInputWorker.cpp
 *
 *  Created on: 27.11.2013
 *      Author: jung
 */

#include "WaitForInputWorker.h"


dsa::WaitForInputWorker::WaitForInputWorker() {
	finished_ = false;
	keyPressed_ = false;
}

dsa::WaitForInputWorker::~WaitForInputWorker() {
}

void dsa::WaitForInputWorker::finish() {
	finished_ = true;
}

bool dsa::WaitForInputWorker::wasKeyPressed() {
	mtx_.lock();
	bool result = keyPressed_;
	mtx_.unlock();
	return(result);
}

bool dsa::WaitForInputWorker::isFinished() {
	mtx_.lock();
	bool result = finished_;
	mtx_.unlock();
	return(result);
}

void dsa::WaitForInputWorker::run() {
	while(!finished_) {
		std::string tmp("");
		std::getline(std::cin, tmp);
		mtx_.lock();
		keyPressed_ = !keyPressed_;
		if(tmp.find('q') != std::string::npos) {
			finished_ = true;
		}
		mtx_.unlock();
	}
}

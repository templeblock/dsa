/*
 * WaitForInputWorker.h
 *
 *  Created on: 27.11.2013
 *      Author: jung
 */

#ifndef WAITFORINPUTWORKER_H_
#define WAITFORINPUTWORKER_H_

#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

namespace dsa {

class WaitForInputWorker {
	bool finished_;
	bool keyPressed_;
	boost::mutex mtx_; // explicit mutex declaration

public:
	WaitForInputWorker();
	virtual ~WaitForInputWorker();

	void finish();
	bool isFinished();
	bool wasKeyPressed();
	void run();
};

} /* namespace dsa */
#endif /* WAITFORINPUTWORKER_H_ */

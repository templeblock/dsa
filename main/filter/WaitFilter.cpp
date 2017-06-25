/*
 * WaitFilter.cpp
 *
 *	This filter does nothing but wait to sync to a specific framerate, where possible. Since we cannot travel back
 *	in time, we can only sync to the future (i.e. wait until sync is restored).
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "WaitFilter.h"

/**
 * Constructor
 */
dsa::WaitFilter::WaitFilter() {
	waitForInput_ = true;
	anyKey_ = true;
	key_ = '\0';
	last_ = 0;
	msPerFrame_ = 0;
}

/**
 * Constructor
 *
 * Wait until the given key is pressed.
 *
 * @param key: key to be pressed to continue;
 *
 */
dsa::WaitFilter::WaitFilter(const char key) {
	waitForInput_ = true;
	anyKey_ = false;
	key_ = key;
	last_ = 0;
	msPerFrame_ = 0;
}

/**
 * Constructor
 *
 * Wait to sync to a specific framerate.
 *
 * @param fps: the framerate
 */
dsa::WaitFilter::WaitFilter(const float fps) {
	waitForInput_ = false;
	anyKey_ = false;
	key_ = '\0';
	last_ = clock();
	msPerFrame_ = CLOCKS_PER_SEC/fps;
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::WaitFilter::~WaitFilter() {
}

/**
 * An ImageFilter is considered ready, when it's input image exists and has data
 *
 * @return true if we have a real image to process, false otherwise
 */
bool dsa::WaitFilter::isReady() const {
	return(true);
}

/**
 * Start waiting. Continue depending on configuration either after a key is pressed (any key by default) or enough time has passed
 * to not exceed the framerate.
 */
void dsa::WaitFilter::evaluate() {
	if(waitForInput_) {
		if(anyKey_)
			cv::waitKey(0);
		else {
			char key;
			while ((key = (char)cv::waitKey(100)) != key_);
		}
	} else {
		clock_t now = clock();
		int waitFor = msPerFrame_ - (int)(now-last_);
		if(waitFor > 0)
		    boost::this_thread::sleep(boost::posix_time::milliseconds(waitFor));
		last_ = now;
	}
}

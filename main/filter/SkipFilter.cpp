/*
 * SkipFilter.h
 *
 * 	Only pass-through n out of m frames
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "SkipFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::SkipFilter::SkipFilter(UINT32 m, UINT32 n) {
	m_ = m;
	n_ = n;
	run_ = 1;
	div_ = n/m;
	lastExec_ = 0;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::SkipFilter::~SkipFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::SkipFilter::init() {
	assert(n_ < m_);
}

/**
 * When filter is ready, determine whether to pass-through the image or not. The idea is, that we need to map n values to the [1;m] interval,
 * so we multiply the current run number by the fraction of n/m and round it to the nearest integer value. If we passed the image through
 * for this value already, we do not pass again, but skip it instead. Therefore after m iterations we have passed through exactly m*n/m = n frames),
 * which are distributed fairly over the entire interval. This assignment may not always be optimal regarding the distance between the skipped frames,
 * but it is very easy to compute.
*/
void dsa::SkipFilter::evaluate() {
	UINT32 currentIntMap = round(run_*div_);
	if(currentIntMap > lastExec_)	// continue
		lastExec_ = currentIntMap;
	else	// skip
		dst = cv::Mat();
	run_++;
	if(run_ > m_) { // start at the beginning (prevents overflow of run)
		run_ = 1;
		lastExec_ = 0;
	}
}

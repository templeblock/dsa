/*
 * Grabilter.cpp
 *
 *	Grabs an image from a Videohub
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef GRABFILTER_H_
#define GRABFILTER_H_

#include "Filter.h"
#include <opencv2/opencv.hpp>

namespace dsa {

class GrabFilter : public Filter {

protected:

	DataHub* input_;
	AbstractSensorData *data;

public:

	clock_t dataTimestamp;

	GrabFilter(DataHub &dataHub);
	virtual ~GrabFilter();

	virtual void preprocessing();
	virtual void apply();
	virtual void bypass() = 0;
	virtual void postprocessing();
	void setInput(DataHub* const dataHub);

	bool isReady() const = 0;
};

}
#endif /* GRABFILTER_H_ */

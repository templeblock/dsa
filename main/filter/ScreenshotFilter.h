/*
 * ScreenshotFilter.h
 *
 * 	Writes a video file of the incoming images. Please note that this can only work, if all further incoming
 * 	images have the same properties as the first image, i.e same size and number of channels, hence you cannot.
 * 	change the properties of the videostream on-the-fly.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef SCREENSHOTFILTER_H_
#define SCREENSHOTFILTER_H_

#include <ctime>
#include <generic/helper.h>
#include "EvaluationFilter.h"
#include <sensordata/ImageWriter.h>

namespace dsa {

class ScreenshotFilter : public EvaluationFilter {

	void evaluate();

	clock_t prevTimestamp_;
	UINT32 run_;

protected:

	ImageWriter writer_;
	std::string basename_;
	std::string extension_;
	char key_;

	void init();

public:

	ScreenshotFilter(const std::string basename="snapshot_", const std::string extension="jpg", char key='s');
	virtual ~ScreenshotFilter();

};

}
#endif /* SCREENSHOTFILTER_H_ */

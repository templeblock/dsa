/*
 * VideoWriterFilter.h
 *
 * 	Writes a video file of the incoming images. Please note that this can only work, if all further incoming
 * 	images have the same properties as the first image, i.e same size and number of channels, hence you cannot.
 * 	change the properties of the videostream on-the-fly.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef VIDEOWRITERFILTER_H_
#define VIDEOWRITERFILTER_H_

#include "EvaluationFilter.h"
#include <sensordata/VideoWriter.h>

namespace dsa {

class VideoWriterFilter : public EvaluationFilter {

	void evaluate();

protected:

	bool isColor_;
	int fourcc_;
	double fps_;
	std::string filename_;
	VideoWriter recorder_;

	void init();

public:

	VideoWriterFilter(const std::string &filename, int fourcc, double fps, bool isColor=true);
	virtual ~VideoWriterFilter();

};

}
#endif /* VIDEOWRITERFILTER_H_ */

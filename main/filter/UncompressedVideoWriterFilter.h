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

#ifndef UNCOMPRESSEDVIDEOWRITERFILTER_H_
#define UNCOMPRESSEDVIDEOWRITERFILTER_H_

#include "EvaluationFilter.h"
#include <sensordata/VideoWriter.h>
#include <generic/types.h>
#include <generic/helper.h>

static const int MAX_VIDEO_SIZE = 2000000;	// max video size in kilobytes

namespace dsa {

class UncompressedVideoWriterFilter : public EvaluationFilter {

	void evaluate();

protected:

	int currentVideoSize_;
	int currentFileNr_;
	double fps_;
	std::string filename_;
	VideoWriter recorder_;

	void init();

public:

	UncompressedVideoWriterFilter(std::string filename, double fps);
	virtual ~UncompressedVideoWriterFilter();
};

}
#endif /* UNCOMPRESSEDVIDEOWRITERFILTER_H_ */

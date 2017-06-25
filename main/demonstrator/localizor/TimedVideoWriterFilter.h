/*
 * TimedVideoWriterFilter.h
 *
 *  Created on: 11.04.2013
 *      Author: Bigontina
 */

#ifndef TIMEDVIDEOWRITERFILTER_H_
#define TIMEDVIDEOWRITERFILTER_H_

#include <time.h>
#include <filter/EvaluationFilter.h>
#include <sensordata/VideoWriter.h>

namespace dsa {

class TimedVideoWriterFilter : public EvaluationFilter {

	bool isColor_;
	int fourcc_;
	double fps_;
	clock_t prevTime_;
	float msPerFrame_;
	std::string filename_;
	VideoWriter recorder_;

	void init();
	void evaluate();

public:

	//bool* isColor;
	//int* fourcc;
	//double* fps;
	//std::string* filename;

	TimedVideoWriterFilter(const std::string &filename, int fourcc, double fps, bool isColor=true);
	virtual ~TimedVideoWriterFilter();

};

}
#endif /* TIMEDVIDEOWRITERFILTER_H_ */

/*
 * EvaluationLogger.h
 *
 * 	See
 *
 *	http://docs.opencv.org/modules/imgproc/doc/histograms.html#equalizehist
 *
 *	for details on the image filter.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef EVALUATIONLOGGER_H_
#define EVALUATIONLOGGER_H_

#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <aruco/aruco.h>
#include <generic/definitions.h>
#include <demonstrator/ForkInfo.h>
#include <demonstrator/BarcodeResult.h>
#include <demonstrator/LocalizationResult.h>
#include <demonstrator/ForkHeightResult.h>
#include <filter/helper/WaitForInputWorker.h>


namespace dsa {

class EvaluationLogger {

	int lastTimestamp_;
	bool toggle_;
	int toggleCount_;
	std::ofstream outfile_;
	WaitForInputWorker *worker_;

	void init();

public:

	void log(LocalizationResult *localizationResult, ForkHeightResult *forkHeightResult);
	void log(BarcodeResult *barcodeResult);
	void log(ForkInfo* forkInfo);

	EvaluationLogger(std::string &filename, WaitForInputWorker *worker);
	virtual ~EvaluationLogger();
};

}
#endif /* EVALUATIONLOGGER_H_ */

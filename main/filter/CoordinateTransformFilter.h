/*
 * CoordinateTransformFilter.h
 *
 *	Performs a coordinate transformation.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef COORDINATETRANSFORMFILTER_H_
#define COORDINATETRANSFORMFILTER_H_

#include <generic/types.h>
#include "EvaluationFilter.h"

namespace dsa {

class CoordinateTransformFilter : public EvaluationFilter {

	bool useTranslationOffset_;
	bool useRotationOffset_;
	cv::Mat translationTransformer_;
	cv::Mat rotationTransformer_;

	void init();
	float mapAngle(float angle);
	void evaluate();

public:

	cv::Mat transPosition;
	cv::Mat transRotation;
	cv::Mat *position;
	cv::Mat *rotation;
	cv::Mat *translationTransformer;
	cv::Mat *rotationTransformer;

	CoordinateTransformFilter();
	CoordinateTransformFilter(cv::Mat &translation, cv::Mat &rotation, bool useTranslationOffset=false, bool useRotationOffset=false);
	virtual ~CoordinateTransformFilter();

	void preprocessing();
	void postprocessing();
};

}
#endif /* COORDINATETRANSFORMFILTER_H_ */

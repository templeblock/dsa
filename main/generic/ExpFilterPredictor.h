/*
 * ExpFilterPredictor.h
 *
 *  Created on: 05.04.2012
 *      Author: jung
 */

#ifndef EXPFILTERPREDICTOR_H_
#define EXPFILTERPREDICTOR_H_

#include "Predictor.h"

namespace dsa {

template <class T>
class ExpFilterPredictor : protected Predictor<T> {
protected:
	T mean_;
	T meanSmooth_;
	float alpha_;

public:
	ExpFilterPredictor(const float alpha) : Predictor<T>(1) {
		Predictor<T>::missesUntilReset = 3;
		this->alpha_ = alpha;
	}

	virtual ~ExpFilterPredictor() {
	}

	virtual const T next() {
		T prev = Predictor<T>::history.read();
		if(!Predictor<T>::isInitialized()) {
			mean_ = prev;
			meanSmooth_ = prev;
			return(prev);
		}
		mean_ = alpha_*prev + (1 - alpha_)*mean_;
		meanSmooth_ = alpha_*mean_ + (1 - alpha_)*meanSmooth_;

		T element = 2*mean_ - meanSmooth_ + (alpha_/(1.0f - alpha_))*(mean_ - meanSmooth_);

		return(element);
	}

	virtual void feed(T& element) {
		if(!Predictor<T>::isInitialized()) {
			Predictor<T>::feed(element);
			next();
			Predictor<T>::initialized = true;
		} else
			Predictor<T>::feed(element);
	}

	virtual void reset() {
		Predictor<T>::reset();
	}

	virtual bool isInitialized() {
		return(Predictor<T>::isInitialized());
	}

	virtual void missed() {
		Predictor<T>::missed();
	}

	virtual void hit() {
		Predictor<T>::hit();
	}
};

}
#endif /* EXPFILTERPREDICTOR_H_ */

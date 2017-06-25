/*
 * VectorPredictor.h
 *
 *  Created on: 05.04.2012
 *      Author: jung
 */

#ifndef VECTORPREDICTOR_H_
#define VECTORPREDICTOR_H_

#include "Predictor.h"

namespace dsa {

template <class T>
class VectorPredictor : protected Predictor<T> {
public:
	VectorPredictor() : Predictor<T>(2) {
		Predictor<T>::missesUntilReset_ = 2;
	}

	virtual ~VectorPredictor() {
	}

	virtual const T next() {
		const std::vector<T>* prev = Predictor<T>::history_.readLastN(2);
		T element = 2 * prev->at(0) - prev->at(1);
		delete prev;
		return(element);
	}

	virtual void feed(T& element) {
		Predictor<T>::feed(element);
		if(Predictor<T>::history_.getSize() == 1) {
			Predictor<T>::history_.write(element);
			Predictor<T>::initialized_ = true;
		}
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
#endif /* VECTORPREDICTOR_H_ */

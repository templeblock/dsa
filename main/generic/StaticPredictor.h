/*
 * VectorPredictor.h
 *
 *  Created on: 05.04.2012
 *      Author: jung
 */

#ifndef STATICPREDICTOR_H_
#define STATICPREDICTOR_H_

#include "Predictor.h"

namespace dsa {

template <class T>
class StaticPredictor : protected Predictor<T> {
public:
	StaticPredictor() : Predictor<T>(1) {
		Predictor<T>::missesUntilReset = UINT_MAX;
	}

	virtual ~StaticPredictor() {
	}

	virtual const T next() {
		T element = Predictor<T>::history.read();
		return(element);
	}

	virtual void feed(T& element) {
		Predictor<T>::feed(element);
		Predictor<T>::initialized = true;
	}

	virtual void reset() {
		Predictor<T>::reset();
	}

	virtual bool isInitialized() {
		return(Predictor<T>::isInitialized());
	}

	virtual void missed() {
		reset();
	}

	virtual void hit() {
		reset();
	}
};

}
#endif /* STATICPREDICTOR_H_ */

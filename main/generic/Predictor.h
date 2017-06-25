/*
 * Predictor.h
 *
 *  Created on: 03.04.2012
 *      Author: jung
 */

#ifndef PREDICTOR_H_
#define PREDICTOR_H_

#include "QueueBuffer.h"

namespace dsa {

template <class T>
class Predictor {
protected:
	bool initialized_;
	UINT32 continuousMisses_;
	UINT32 missesUntilReset_;
	QueueBuffer<T> history_;
public:
	Predictor(const size_t size) {
		continuousMisses_ = 0;
		missesUntilReset_ = 5;
		initialized_ = false;
		history_ = QueueBuffer<T>(size);
	}

	virtual ~Predictor() {
	}

	virtual const T next() = 0;

	virtual void feed(T& element) {
		history_.write(element);
	}

	virtual void reset() {
		initialized_ = false;
		continuousMisses_ = 0;
		history_.free();
	}

	virtual bool isInitialized() const {
		return(initialized_);
	}

	virtual void missed() {
		continuousMisses_++;
		if(continuousMisses_ >= missesUntilReset_) {
			reset();
		}
	}

	virtual void hit() {
		continuousMisses_ = 0;
	}
};

}
#endif /* PREDICTOR_H_ */

/*
 * MeanQueueBuffer.h
 *
 *  Created on: 03.04.2012
 *      Author: jung
 */

#ifndef MEANQUEUEBUFFER_H_
#define MEANQUEUEBUFFER_H_

#include <list>
#include <vector>
#include "types.h"
#include "QueueBuffer.h"

namespace dsa {

template <class T>
class MeanQueueBuffer : public QueueBuffer<T> {

private:
	T sum_;

public:
	MeanQueueBuffer() : QueueBuffer<T>() {
		sum_ = 0;
	}

	MeanQueueBuffer(const size_t size) : QueueBuffer<T>(size) {
		sum_ = 0;
	}

	virtual ~MeanQueueBuffer() {
	}

	void write(const T& element) {
		if(QueueBuffer<T>::isFull()) {
			sum_ -= QueueBuffer<T>::buffer_.back();
			QueueBuffer<T>::buffer_.pop_back();
		}
		sum_ += element;
		QueueBuffer<T>::buffer_.push_front(element);
	}

	void free() {
		sum_ = 0;
		QueueBuffer<T>::free();
	}

	void resize(size_t size) {
		int nDelete = this->size_ - size;
		for(UINT32 i = 0; i < nDelete; i++) {
			sum_ -= QueueBuffer<T>::buffer_.back();
			QueueBuffer<T>::buffer_.pop_back();
		}
		this->size_ = size;
	}

	const T getMean() const {
		return(sum_/QueueBuffer<T>::getSize());
	}

	const T getRest() const {
		return(sum_%QueueBuffer<T>::getSize());
	}

	const T getVariance() const {
		if(QueueBuffer<T>::getSize() < 2)
			return(0);
		else {
			T mean = getMean();
			T sum = 0;

			size_t n = 0;
			typename std::list<T>::const_iterator it;
			for(it = QueueBuffer<T>::buffer_.begin(); it != QueueBuffer<T>::buffer_.end(); it++) {
				sum += (*it-mean)*(*it-mean);
				n++;
			}

			return(sum/(n-1));
		}
	}
};

}
#endif /* MEANQUEUEBUFFER_H_ */

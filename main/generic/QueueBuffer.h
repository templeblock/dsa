/*
 * QueueBuffer.h
 *
 *  Created on: 03.04.2012
 *      Author: jung
 */

#ifndef QUEUEBUFFER_H_
#define QUEUEBUFFER_H_

#include <list>
#include <iterator>
#include <vector>
#include "types.h"

namespace dsa {

template <class T>
class QueueBuffer {

protected:
	static const size_t defaultBufferSize = 10;
	size_t size_;
	std::list<T> buffer_;	// the buffer

public:
	QueueBuffer() {
		size_ = defaultBufferSize;
		std::list<T> buffer;	// the buffer
	}

	QueueBuffer(const size_t size) {
		this->size_ = size;
		buffer_ = std::list<T>();
	}

	virtual ~QueueBuffer() {
		free();
	}

	void write(const T& element) {
		if(isFull())
			buffer_.pop_back();
		buffer_.push_front(element);
	}

	const T read() const {
		return(buffer_.front());
	}

	const T read(UINT32 n) const {
		if(buffer_.size() < n)
			return(NULL);
		else {
			typename std::list<T>::const_iterator buffer_it;
			if(n < buffer_.size()/2) {
				buffer_it = buffer_.begin();
				for(UINT32 pos = 0; pos < n; pos++);
					buffer_it++;
			} else {
				buffer_it = buffer_.end();
				for(UINT32 pos = buffer_.size()-1; pos > n; pos--);
					buffer_it--;
			}
			return(*buffer_it);
		}
	}

	std::vector<T>* readLastN(UINT32 nElements) const {
		UINT32 n = std::min((unsigned)nElements, (unsigned)buffer_.size());
		std::vector<T>* vec;
		if(nElements >= buffer_.size())
			vec = new std::vector<T>(buffer_.begin(), buffer_.end());
		else {
			vec = new std::vector<T>(n);
			std::_List_const_iterator<T> it = buffer_.begin();
			for(UINT32 i = 0; i < n; i++) {
				vec->at(i) = *it;
				it++;
			}
		}

		return(vec);
	}

	void free() {
		buffer_.clear();
	}

	void resize(size_t size) {
		int nDelete = this->size_ - size;
		for(UINT32 i = 0; i < nDelete; i++)
			buffer_.pop_back();
		this->size_ = size;
	}

	inline bool isEmpty() const {
		return(getSize() == 0);
	}

	inline bool isFull() const {
		return(getSize() == size_);
	}

	inline size_t getSize() const {
		return(buffer_.size());
	}
};

}
#endif /* QUEUEBUFFER_H_ */

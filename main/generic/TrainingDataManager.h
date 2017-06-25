/*
 * TrainingDataManager.h
 *
 * Manages training data for machine learning algorithms
 *
 *  Created on: 05.04.2012
 *      Author: jung
 */

#ifndef TRAININGDATAMANAGER_H_
#define TRAININGDATAMANAGER_H_

#include <list>
#include <vector>
#include <iterator>

namespace dsa {

template <class T>
class TrainingDataManager {

	bool replaceRand_;
	UINT32 maxElements_;
	std::string filename_;
	std::list<T> data_;	// FIFO Queue

public:
	TrainingDataManager(const std::string filename, const UINT32 maxElements=5000, const bool replaceRand=true) {
		filename_ = filename;
		maxElements_ = maxElements;
		replaceRand_ = replaceRand;

		// read as vector, because a vector can be read more easily from xml file
		std::vector<T> tmp = std::vector<T>();

		// read positives and negatives from xml when possible
		if(filename.length() > 0) {
			cv::FileStorage fs(filename_, cv::FileStorage::READ);
			if(fs.isOpened())
				fs["data"] >> tmp;
			fs.release();
		}

		// copy to list and skip oldest elements exceeding size constraint
		if(tmp.size() <= maxElements_)
			data_ = std::list<T>(tmp.begin(), tmp.end());
		else {
			typename std::vector<T>::iterator it = tmp.begin();
			for(size_t i = 0; i < maxElements_-tmp.size(); i++)
				it++;
			data_ = std::list<T>(it, tmp.end());
		}
	}

	virtual ~TrainingDataManager() {
		// write trainingdata to file
		cv::FileStorage fs(filename_, cv::FileStorage::WRITE);
		bool success = fs.isOpened();
		if(success && data_.size() > 0) {
			// write as vector, because a vector can be stored more easily into an xml file
			std::vector<T> tmp = std::vector<T>(data_.begin(), data_.end());
			fs << "data" << tmp;
		}
		fs.release();
	}

	virtual bool isFull() {
		return(data_.size() >= maxElements_);
	}

	virtual void add(T element) {
		data_.push_back(element);
		if(isFull()) {
			if(replaceRand_) {
				// determine random position, remove
				size_t pos = (float)(rand()*data_.size())/RAND_MAX;
				typename std::list<T>::iterator it;
				if(pos < data_.size()/2) {	// from the beginning
					it = data_.begin();
					for(size_t i = 0; i < pos; i++);
						it++;
				} else {	// backwards instead
					pos = data_.size() - pos;
					it = data_.end();
					for(size_t i = 0; i < pos; i++);
						it--;
				}
				data_.erase(it);
			} else
				data_.pop_front();	// remove oldest entry
		}
	}

	virtual std::list<T>& getData() {
		return(data_);
	}

	virtual void reset() {
		data_.clear();
	}
};

}
#endif /* TRAININGDATAMANAGER_H_ */

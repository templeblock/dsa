/*
 * Filter.h
 *
 * 	Baseclass of ImageFilter and EvaluationFilter (and potentially other filter-related classes)
 *
 *  Created on: 27.06.2012
 *      Author: jung
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <sensordata/DataHub.h>

namespace dsa {

class Filter {

protected:

	bool isActive_;

	Filter* predecessor_;
	Filter* successor_;

public:

	// create standalone filter
	Filter();
	// Destructor
	virtual ~Filter();

	bool isActive() const;
	void setActive(const bool isActive);

	virtual void preprocessing() = 0;
	// apply the filter. must be implemented by derived classes
	virtual void apply() = 0;
	// bypass the filter. must be implemented by derived classes
	virtual void bypass() = 0;
	virtual void postprocessing() = 0;

	virtual bool isReady() const = 0;

	Filter* getPredecessor();
	Filter* getSuccessor();

	void setPredecessor(Filter* filter);
	void setSuccessor(Filter* filter);
};

}
#endif /* FILTER_H_ */

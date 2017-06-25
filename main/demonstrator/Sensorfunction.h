/*
 * Sensorfunction.h
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#ifndef SENSORFUNCTION_H_
#define SENSORFUNCTION_H_

#include <generic/types.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

namespace dsa {

class Sensorfunction {
	SensorType type_;	// type of sensor

protected:
	boost::mutex mtx_; // explicit mutex declaration

public:
	Sensorfunction(SensorType type);
	virtual ~Sensorfunction();

	SensorType getType();

	virtual void run() = 0;
	virtual bool calibrate() = 0;
};

}
#endif /* SENSORFUNCTION_H_ */

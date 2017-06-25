/*
 * AbstractSensorData.h
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#ifndef ABSTRACTSENSORDATA_H_
#define ABSTRACTSENSORDATA_H_

#include <ctime>

namespace dsa {

class AbstractSensorData {

	void init(const clock_t grabTimestamp, const clock_t dataTimestamp);

protected:
	bool copy_;				// data is a copy (true) or shared reference to the data (false)
	clock_t grabTimestamp_;	// time at which we grabbed the data
	clock_t dataTimestamp_;	// time at which the data was measured

public:
	AbstractSensorData();
	AbstractSensorData(const clock_t grabTimestamp);
	AbstractSensorData(const clock_t grabTimestamp, const clock_t dataTimestamp);
	AbstractSensorData(const AbstractSensorData& copy);
	virtual ~AbstractSensorData();
	clock_t getGrabTimestamp() { return(grabTimestamp_); };
	clock_t getDataTimestamp() { return(dataTimestamp_); };
};

}
#endif /* ABSTRACTSENSORDATA_H_ */

/*
 * DataHub.h
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#ifndef DATAHUB_H_
#define DATAHUB_H_

#include "AbstractSensorData.h"
#include <generic/helper.h>

namespace dsa {

class DataHub {

protected:
	static const int BUFFER_SIZE = 25;
	clock_t lastGrabbed_;
	AbstractSensorData original_;

	virtual void grabData();
	bool needsGrabbing(const clock_t);

public:

	DataHub();
	virtual ~DataHub();
	virtual AbstractSensorData* getData(const bool copy=true) = 0;
	virtual AbstractSensorData* getData(const clock_t clock_t, const bool copy=true) = 0;
};

}
#endif /* DATAHUB_H_ */

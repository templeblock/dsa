/*
 * AutoWhiteBalanceFilter.h
 *
 * Some camera's can do this automatically, but we have to assume your's can't.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef AUTOWHITEBALANCEFILTER_H_
#define AUTOWHITEBALANCEFILTER_H_

#include <generic/types.h>
#include "ImageFilter.h"

namespace dsa {

class AutoWhiteBalanceFilter : public ImageFilter {
	bool assumeGrayWorld_;
	void init();

public:

	AutoWhiteBalanceFilter(bool assumeGrayWorld=false);
	virtual ~AutoWhiteBalanceFilter();

	void apply();
};

}
#endif /* AUTOWHITEBALANCEFILTER_H_ */

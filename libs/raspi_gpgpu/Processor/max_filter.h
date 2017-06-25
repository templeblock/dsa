/*
 * max_filter.h
 *
 *  Created on: 18 mar 2016
 *      Author: gregStudent
 */

#ifndef MAX_FILTER_H_
#define MAX_FILTER_H_

#include "../egl_include.h"
#include "basic_filter.h"


namespace raspi_gpgpu
{
 class MaxProcessor : public BasicFilter
 {
 public:
	 MaxProcessor();

	 virtual const char *GetProcName() { return ("MaxProcessor"); }

	 virtual int Initialize(int inputWidth, int inputHeight, int outputWidth, int outputHeight, unsigned int order, bool externalInput) ;

	 void DoRender();

 private:
     static const char *_fragmentShaderSource;
 };
}



#endif /* MAX_FILTER_H_ */

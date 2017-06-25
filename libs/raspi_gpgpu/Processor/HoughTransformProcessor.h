/*
 * HoughTransformProcessor.h
 *
 *  Created on: 1 lut 2016
 *      Author: gregStudent
 */

#ifndef HOUGHTRANSFORMPROCESSOR_H_
#define HOUGHTRANSFORMPROCESSOR_H_

#include "../egl_include.h"
#include "complex_filter.h"
#include "hough_transform.h"


namespace raspi_gpgpu
{
  class HoughTransformProcessor : public ComplexFilter {
  public:
	  HoughTransformProcessor()
	  {

		  raspi_gpgpu::HoughTransform *h1 = new raspi_gpgpu::HoughTransform(1);
		  //raspi_gpgpu::HoughTransform *h2 = new raspi_gpgpu::HoughTransform(2);

		  _processorPipeline.push_back(h1);
     	  //_processorPipeline.push_back(h2);
		  ComplexInit();
	   }
};
}



#endif /* HOUGHTRANSFORMPROCESSOR_H_ */

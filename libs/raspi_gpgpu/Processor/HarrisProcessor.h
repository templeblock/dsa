/*
 * HarrisProcessor.h
 *
 *  Created on: 23 lut 2016
 *      Author: gregStudent
 */

#ifndef HARRISPROCESSOR_H_
#define HARRISPROCESSOR_H_


#include "../egl_include.h"
#include "complex_filter.h"
#include "HarrisGL.h"

namespace raspi_gpgpu
{

class HarrisProcessor : public ComplexFilter {
  public:
	HarrisProcessor()
	  {

		raspi_gpgpu::HarrisGL *h1 = new raspi_gpgpu::HarrisGL(1);
		raspi_gpgpu::HarrisGL *h2 = new raspi_gpgpu::HarrisGL(2);
		raspi_gpgpu::HarrisGL *h3 = new raspi_gpgpu::HarrisGL(3);
		raspi_gpgpu::HarrisGL *h4 = new raspi_gpgpu::HarrisGL(4);
		raspi_gpgpu::HarrisGL *h5 = new raspi_gpgpu::HarrisGL(5);

		 _processorPipeline.push_back(h1);
		 _processorPipeline.push_back(h2);
		 _processorPipeline.push_back(h3);
		 _processorPipeline.push_back(h4);
		 _processorPipeline.push_back(h5);

		  ComplexInit();
	   }
};

}
#endif /* HARRISPROCESSOR_H_ */

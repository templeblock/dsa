/*
 * CannyProcessor.h
 *
 *  Created on: 30 sty 2016
 *      Author: gregStudent
 */

#ifndef CANNYPROCESSOR_H_
#define CANNYPROCESSOR_H_


#include "../egl_include.h"
#include "complex_filter.h"
#include "Canny.h"


namespace raspi_gpgpu
{
  class CannyProcessor : public ComplexFilter {
  public:
	  CannyProcessor()
	  {
		  // horizontal blurr pass
		  raspi_gpgpu::Canny *cn1 = new raspi_gpgpu::Canny(1);
		  // vertical blurr pass
		  raspi_gpgpu::Canny *cn2 = new raspi_gpgpu::Canny(2);

		  raspi_gpgpu::Canny *cn3 = new raspi_gpgpu::Canny(3);
		  raspi_gpgpu::Canny *cn4 = new raspi_gpgpu::Canny(4);
		  raspi_gpgpu::Canny *cn5 = new raspi_gpgpu::Canny(5);

		  _processorPipeline.push_back(cn1);
     	  _processorPipeline.push_back(cn2);
     	  _processorPipeline.push_back(cn3);
     	  _processorPipeline.push_back(cn4);
     	  _processorPipeline.push_back(cn5);

		  ComplexInit();
	   }
};
}
#endif /* CANNYPROCESSOR_H_ */

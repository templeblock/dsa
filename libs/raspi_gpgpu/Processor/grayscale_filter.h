/*
 * grayscale_filter.h
 *
 *  Created on: 12 sty 2016
 *      Author: gregStudent
 */

#ifndef GRAYSCALE_FILTER_H_
#define GRAYSCALE_FILTER_H_

#include "../egl_include.h"
#include "basic_filter.h"


namespace raspi_gpgpu
{
 class GrayscaleProcessor : public BasicFilter
 {
 public:
	 GrayscaleProcessor();

	 virtual const char *GetProcName() { return ("SchwarzWeiﬂ"); }

	 virtual int Initialize(int inputWidth, int inputHeight, int outputWidth, int outputHeight, unsigned int order, bool externalInput) ;

	 void DoRender();

 private:
     static const char *_fragmentShaderSource;
     static const GLfloat _grayscaleConvVecRGB[3];


     GLint _shaderParameter;
     GLfloat _grayscaleConversion[3];
 };
}


#endif /* GRAYSCALE_FILTER_H_ */

/*
 * HarrisGL.h
 *
 *  Created on: 23 lut 2016
 *      Author: gregStudent
 */

#ifndef HARRISGL_H_
#define HARRISGL_H_

#include "../egl_include.h"
#include "basic_filter.h"


namespace raspi_gpgpu
{
	class HarrisGL : public BasicFilter
	{
	public:

		HarrisGL(int pass) : BasicFilter(),
		  _pass(pass)
			  {
				  assert(_pass > 0 && _pass < 10);
			  }

		  virtual const char *GetProcName() { return ("HarrisGL filter"); }

		  virtual int Initialize(int inputWidth, int inputHeight, int outputWidth, int outputHeight, unsigned int order, bool externalInput) ;

		  void DoRender();

		  virtual void InitFrameBufferTexture();

	private:
		int _pass;

		 GLint _shParamStep;


		static const char *_fragmentHarris1Src;
		static const char *_fragmentHarris2Src;
		static const char *_fragmentHarris3Src;
		static const char *_fragmentHarris4Src;
		static const char *_fragmentHarris5Src;


	};
}



#endif /* HARRISGL_H_ */

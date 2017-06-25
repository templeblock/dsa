/*
 * adaptive_threshold.h
 *
 *  Created on: 9 sty 2016
 *      Author: gregStudent
 */

#ifndef RASPI_ADAPTIVE_THRESHOLD_H_
#define RASPI_ADAPTIVE_THRESHOLD_H_

#include "../egl_include.h"
#include "basic_filter.h"

namespace raspi_gpgpu
{
  class AdaptiveThreshold : public BasicFilter
  {
  public:

	  AdaptiveThreshold(int pass) : BasicFilter(),
	  _pass(pass),
	  _Dx(0.0f),
	  _Dy(0.0f)
	      {
	          assert(_pass == 1 || _pass == 2);
	      }

	  virtual const char *GetProcName() { return ("AdaptiveThreshold"); }

	  virtual int Initialize(int inputWidth, int inputHeight, int outputWidth, int outputHeight, unsigned int order, bool externalInput) ;

	  void DoRender();

	  virtual void InitFrameBufferTexture();

  private:
      int _pass;

  	  GLint _shParameterd;

  	  float _Dx;
  	  float _Dy;

      static const char *_fragmentAdaptThresh1Src;
      static const char *_fragmentAdaptThresh2Src;

  };
}

#endif /* ADAPTIVE_THRESHOLD_H_ */

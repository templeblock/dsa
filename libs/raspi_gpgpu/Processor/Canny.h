/*
 * Canny.h
 *
 *  Created on: 30 sty 2016
 *      Author: gregStudent
 
 code in :
 static const char *_fragmentCanny1Src;
      static const char *_fragmentCanny2Src;
      static const char *_fragmentCanny3Src;
      static const char *_fragmentCanny4Src;
      static const char *_fragmentCanny5Src;
	  
used partly from GPUIMAGE library . GPUIMAGE library licence:
	  
 Copyright (c) 2012, Brad Larson, Ben Cochran, Hugues Lismonde, Keitaroh Kobayashi, Alaric Cole, Matthew Clark, Jacob Gundersen, Chris Williams.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the GPUImage framework nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#ifndef CANNY_H_
#define CANNY_H_

#include "../egl_include.h"
#include "basic_filter.h"
#include "../raspi_config.h"

namespace raspi_gpgpu
{
  class Canny : public BasicFilter
  {
  public:

	  Canny(int pass) : BasicFilter(),
	  _pass(pass)
	      {
	          assert(_pass > 0 && _pass < 10);
	      }

	  virtual const char *GetProcName() { return ("Canny filter"); }

	  virtual int Initialize(int inputWidth, int inputHeight, int outputWidth, int outputHeight, unsigned int order, bool externalInput) ;

	  void DoRender();

	  virtual void InitFrameBufferTexture();

  private:
      int _pass;

  	  GLint _shParamStep;
     GLint _shParamWidth;
  	  GLint _shParamHeight;
  	  GLint _shParamLT; // lower treshold
  	  GLint _shParamUT; // upper treshold

      static const char *_fragmentCanny1Src;
      static const char *_fragmentCanny2Src;
      static const char *_fragmentCanny3Src;
      static const char *_fragmentCanny4Src;
      static const char *_fragmentCanny5Src;

  };
}

#endif /* CANNY_H_ */

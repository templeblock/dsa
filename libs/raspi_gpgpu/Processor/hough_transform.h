/*
 * hough_transform.h
 *
 *  Created on: 27 sty 2016
 *      Author: gregStudent
 */

#ifndef HOUGH_TRANSFORM_H_
#define HOUGH_TRANSFORM_H_


#include "../egl_include.h"
#include "basic_filter.h"
#include "../raspi_config.h"

namespace raspi_gpgpu
{
  class HoughTransform: public BasicFilter
  {
   public:
	  HoughTransform(int pass) : BasicFilter(),
	  _pass(pass)
      {
          assert(_pass > 0 && _pass < 10);
          _TSCoordsBufferFull  = NULL;
          _textureCoordsBufferFull = NULL;
      }

	  virtual const char *GetProcName() { return ("Hough Transform filter"); }

	  virtual int Initialize(int inputWidth, int inputHeight, int outputWidth, int outputHeight, unsigned int order, bool externalInput) ;

	  void DoRender();

	  virtual void InitFrameBufferTexture();

	  void SetHoughAttributes();

   private :

	  int _pass;
	  static const char *_fragmentHoughTransform1Src;
	  static const char *_fragmentHoughTransform2Src;
	  GLint _shHoughSpace;
  };

}


#endif /* HOUGH_TRANSFORM_H_ */

/*
 * basic_filter.h
 *
 *  Created on: 9 sty 2016
 *      Author: gregStudent
 */

#ifndef BASIC_FILTER_H_
#define BASIC_FILTER_H_

#include "../egl_include.h"
#include "basic_processor.h"
#include "../Tool.h"
#include <cmath>

#define gpgpu_vertexBufferSize 12
#define gpgpu_textureBufferSize 8

namespace raspi_gpgpu
{
  class BasicFilter : public BasicProcessor
  {
public:
	  BasicFilter() : BasicProcessor(),
	  _fragmentShaderSourceCompilation(NULL),
	  _isHoughProc(-1)
	   {};
	  ~BasicFilter();


	  virtual void SetOutputRenderOrientation(int orientation);
	  virtual void UseTexture(GLuint id, GLuint useTextureUnit = 1, GLenum target = GL_TEXTURE_2D);

protected:

	  void InitFilter(const char *filterShaderSource, int o = 0);

	  void DoFilterSetup(const char *filterShaderSource, GLenum target);
	  void InitTextureCoordianates(int setting = 0);
	  void DoRender(){};
	  void RenderPreinit();
	  void RenderSetCoordinates();
	  void RenderDraw();
	  void RenderDispose();
	  int IsHoughProc();
	  void ResetProcVariables();


	  static const  char *_vertexShaderDefault;
	  static const  char *_vertexShaderHough;


	  GLfloat _vertexBuffer[12];
	  GLfloat _textureCoordsBuffer[8];
	  GLfloat* _textureCoordsBufferFull;
	  GLfloat* _TSCoordsBufferFull;
	  const char *_fragmentShaderSourceCompilation;
	  GLint _pShaderPos;
	  GLint _aShaderTextureCoordinates;
	  GLint _aShaderParamATSCoordType;
	  //specific for HoughTransform, attribute for current angle
	  int _isHoughProc;
	  GLint _aVertexAngleParam;
	  float _angleParam;
	  GLuint vBuff[2];
  };
}



#endif /* BASIC_FILTER_H_ */

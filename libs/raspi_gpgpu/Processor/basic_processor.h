/*
 * basic_processor.h
 *
 *  Created on: 9 sty 2016
 *      Author: gregStudent
 */

#ifndef BASIC_PROCESSOR_H_
#define BASIC_PROCESSOR_H_

#include "../egl_include.h"
#include "../openGLES/MemoryTransfer.h"
#include "../openGLES/ShaderFactory.h"
#include "../openGLES/FrameBufferManager.h"

namespace raspi_gpgpu
{
  class BasicProcessor
  {
	  public:
		  BasicProcessor();
		 ~BasicProcessor();

		 virtual int Reinit(int inputWidth, int inputHeight, bool externalInput = false);
		 virtual void Dispose();

		 virtual int Initialize(int inputWidth, int inputHeight, int outputWidth, int outputHeight, unsigned int order, bool externalInput) ;

		 virtual void SetExternalInputFormat(GLenum format) { _inputDataFormat = format; }

		 virtual void SetExternalInputData(const unsigned char *source) ;

		 virtual void InitFrameBufferTexture() ;

		 virtual void DoRender() = 0 ;

		 virtual const char *GetProcessorName() { return "BasicProcessor"; }

		 virtual void UseTexture(GLuint id, GLuint useTexUnit = 1, GLenum target = GL_TEXTURE_2D)  = 0;

		 virtual GLuint GetTextureUnit() { return (_intextureUnit);}

		 //virtual void SetOutputSize(float scale) = 0;

		 virtual void SetOutputSize(int outputWidth, int outputHeight) { _processorOutWidth = outputWidth; _processorOutHeight = outputHeight; }

		// virtual void setOutputRenderOrientation(int orientation) = 0;

		 virtual void GetResultData(unsigned char *source) const ;

		 virtual MemoryTransfer *GetMemoryTransferObject() const;

		 virtual GLuint GetInputTextureId() const { return _inTextureId; }

		 virtual GLuint GetOutputTextureId() const ;

		 virtual int GetOutFrameWidth() const { return _outputFrameWidth; }

		 virtual int GetOutFrameHeigth() const { return _outputFrameHeight; }

		 virtual void PrintInit();

		 virtual void ResetProcVariables() = 0;

  protected:

	// virtual void baseInit(int inW, int inH, unsigned int order, bool prepareForExternalInput = false, int outW = 0, int outH = 0, float scaleFactor = 1.0f);


	    virtual void SetInOutFrameSizes(int inputWidth, int inputHeight, int outputWidth, int outputHeight);

	    virtual void CreateFBO();

	    virtual void InitShader(const char *vertexSource,  const char *fSource, GLenum target);


		static const GLfloat TexCoordArray[];
		static const GLfloat TexCoordDiagonalArray[];

		static  GLfloat Vertices[];

	    FrameBufferManager *_frameBufferObject;
		ShaderFactory *_shader;

	    unsigned int _order;
	    int _rOrientation;

	    GLint _shaderParamUInputTex;
	    GLenum _inputDataFormat;

		int _inFrameWidth;
		int _inFrameHeight;
		GLuint _inTextureId;
	    GLuint _intextureUnit;
	    GLenum _inTextureTarget;

	    int _processorOutWidth;
	    int _processorOutHeight;
	    int _outputFrameWidth;
		int _outputFrameHeight;

  };
}

#endif /* BASIC_PROCESSOR_H_ */

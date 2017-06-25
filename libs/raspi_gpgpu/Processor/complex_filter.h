/*
 * complex_filter.h
 *
 *  Created on: 21 sty 2016
 *      Author: gregStudent
 */

#ifndef COMPLEX_FILTER_H_
#define COMPLEX_FILTER_H_

#include <list>
#include "../egl_include.h"
#include "basic_processor.h"


namespace raspi_gpgpu
{
  class ComplexFilter : public BasicProcessor
  {
  public :


	     virtual ~ComplexFilter();

	     virtual void Dispose();

	     virtual int Initialize(int inputWidth, int inputHeight, int outputWidth, int outputHeight, unsigned int order, bool externalInput) ;

	     virtual int Reinit(int inputWidth, int inputHeight, bool externalInput = false);


	     virtual void SetExternalInputFormat(GLenum format);

	     virtual void SetExternalInputData(const unsigned char *source) ;

	     virtual void InitFrameBufferTexture();

//	     /**
//	      * Print some information about the processor's setup.
//	      */
//	     virtual void printInfo();

	     virtual void SetOutputSize(int outputWidth, int outputHeight);

	     //virtual void SetOutputRenderOrientation(int orientation);

	     virtual int GetOutFrameWidth() const;

	     virtual int GetOutFrameHeigth() const;

	     virtual void GetResultData(unsigned char *source) const;

	     virtual MemoryTransfer *GetMemoryTransferObject() const;

	     virtual GLuint GetInputTextureId() const;

	     virtual GLuint GetOutputTextureId() const;

	     size_t getNumPasses() const { return (_processorPipeline.size()); }

	     std::list<BasicProcessor *> GetProcessorPipeline() const { return (_processorPipeline); }

	     virtual void  UseTexture(GLuint id, GLuint useTexUnit = 1, GLenum target = GL_TEXTURE_2D);

	     virtual GLuint GetTextureUnit();

	     virtual void DoRender();

	     void ResetProcVariables(){};

  protected:

      void ComplexInit();


      std::list<BasicProcessor *> _processorPipeline;
      BasicProcessor *_firstProcessor;
      BasicProcessor *_lastProcessor;
  };
}

#endif /* COMPLEX_FILTER_H_ */

/*
 * Controller.h
 *
 *  Created on: 9 sty 2016
 *      Author: gregStudent
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "../egl_include.h"
#include "../openGLES/MemoryTransfer.h"
#include "basic_filter.h"
#include "grayscale_filter.h"
#include <vector>
#include <list>



namespace raspi_gpgpu
{
 //class Disp;

 class Controller
 {
 public:

	 Controller();

	 static Controller *GetInstance();

	 void Dispose();

	 ~Controller();

	 void Reset();

	 void AddProcessor(BasicProcessor *proc);

	 BasicFilter *CreateRender(int dispWidth = 0, int dispHeight = 0, int orientation = 0);

	 void Init(void *glContext = NULL);

	 bool IsInitialized()
			 {
				if(_renderDisp)
					return (true);
				else
					return (false);
			 }

	 void Prepare(int inputWidth, int inputHeight, GLenum inputFormat = GL_RGBA);

	 BasicFilter *GetRender() const { return (_renderDisp); }

	 MemoryTransfer *GetInMemoryTransfer() const;

	 MemoryTransfer *GetOutMemoryTransfer() const;

	 void SetInputTextureId(GLuint inputTextureId, GLenum inputTextureTarget = GL_TEXTURE_2D);

	 void SetInputData(const unsigned char *source);

	 void Process();

	 GLuint GetOutputTextureId() const { assert(_lastProcessor); return (_lastProcessor->GetOutputTextureId()); }

	 void GetOutputData(unsigned char *buffer);

	 int GetOutputFrameWidth() const { return (_oFrameWidth); }

	 int GetOutputFrameHeight() const { return (_oFrameHeight); }

	 void *GetOpenGLContext() const { return (_openGLContext); }



 private:

	 //void Dispose();
	 static Controller *Instance;

	 void *_openGLContext;

	 std::list<BasicProcessor *> _pipeline;

	 BasicProcessor *_firstProcessor;
	 BasicProcessor *_lastProcessor;

	 BasicFilter *_renderDisp;

	 bool _isInitialized;
	 bool _isPrepared;

	// bool useMipmaps;        // use mipmaps?
	// bool glExtNPOTMipmaps;  // hardware supports NPOT mipmapping?
	// bool inputSizeIsPOT;    // input frame size is POT?

	 int _inputFrameWidth;
	 int _inputFrameHeidth;
	 int _oFrameWidth;
	 int _oFrameHeight;

	 GLuint _inputTextureId;
	 GLenum _inputTextureTarget;
	 GLuint _outputTextureId;
 };
}

#endif /* CONTROLLER_H_ */

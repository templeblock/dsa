/*
 * Controller.cpp
 *
 *  Created on: 9 sty 2016
 *      Author: gregStudent
 */

#include "Controller.h"
#include <string>
#include <algorithm>

using namespace raspi_gpgpu;


raspi_gpgpu::Controller::Controller() {
	_renderDisp = NULL;
	_isInitialized = false;
	_openGLContext = NULL;
	_inputTextureTarget = GL_TEXTURE_2D;
	Reset();
}

Controller *Controller::Instance = NULL;

Controller* Controller::GetInstance() {
	 if (!Controller::Instance) {
		 Controller::Instance = new Controller();
	    }

	    return Controller::Instance;
}

void raspi_gpgpu::Controller::Dispose() {
	  if (_renderDisp) {
	        delete _renderDisp;
	        _renderDisp = NULL;
	    }

	    for (std::list<BasicProcessor *>::iterator it = _pipeline.begin();
	         it != _pipeline.end();
	         ++it)
	    {
	        (*it)->Dispose();
	    }
	    _pipeline.clear();
}

raspi_gpgpu::Controller::~Controller() {
	Dispose();
}

void raspi_gpgpu::Controller::Reset() {
	Dispose();

	_isPrepared = false;
	_inputFrameWidth = _inputFrameHeidth = 0;
	_oFrameWidth = _oFrameHeight = 0;
	_inputTextureId = _outputTextureId = 0;
	_firstProcessor  = NULL;
	_lastProcessor = NULL;
}

void raspi_gpgpu::Controller::AddProcessor(BasicProcessor* p) {
	    _pipeline.push_back(p);
}

BasicFilter* raspi_gpgpu::Controller::CreateRender(int dispWidth, int dispHeight,
		int orientation) {
	assert(!_renderDisp);

	_renderDisp = new raspi_gpgpu::BasicFilter();

	    if (dispWidth > 0 && dispHeight > 0) {
	    	_renderDisp->SetOutputSize(dispWidth, dispHeight);
	    }

	    return (_renderDisp);
}

void raspi_gpgpu::Controller::Init(void* glContext) {
	assert(!_isInitialized);
	_openGLContext = glContext;
	    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE1);

	Tool::LastGLError("Controller", "raspi_gpgpu::Controller::Init");

	_isInitialized = true;
}

void raspi_gpgpu::Controller::Prepare(int inputWidth, int inputHeight, GLenum inputFormat) {
	std::cout<< std::endl<< "pipeline size is " << _pipeline.size();
	    assert(_isInitialized && inputWidth > 0 && inputHeight > 0 && _pipeline.size() > 0);

	    if (_isPrepared && _inputFrameWidth == inputWidth && _inputFrameHeidth == inputHeight) return;   // no change

	    // set input frame size
	    //inputSizeIsPOT = Tools::isPOT(inputWidth) && Tools::isPOT(inputHeight);
	    _inputFrameWidth = inputWidth;
	    _inputFrameHeidth = inputHeight;

	    printf("prepare with input frame size %dx%d , %u processors in pipeline",
	    		_inputFrameWidth, _inputFrameHeidth, (unsigned int)_pipeline.size());

	    // initialize the pipeline
	    BasicProcessor *previousProc = NULL;
	    unsigned int num = 0;
	    int numInitialized = 0;

	    for (std::list<BasicProcessor *>::iterator it = _pipeline.begin();
	         it != _pipeline.end();
	         ++it)
	    {
	    	printf("init proc#%d", num);

	        // find out the input frame size for the proc
	        int pipelineFrameW, pipelineFrameH;

	        if (num == 0) { // special set up for first pipeline processor
	        	_firstProcessor = *it;

	            // first pipeline processor will get input data (e.g. RGBA pixel data)
	        	_firstProcessor->SetExternalInputFormat(inputFormat);

	            // first pipeline's frame size is the input frame size
	            pipelineFrameW = _inputFrameWidth;
	            pipelineFrameH = _inputFrameHeidth;
	        } else {
	            // subsequent pipeline's frame size is the previous processor's output frame size
	            pipelineFrameW = previousProc->GetOutFrameWidth();
	            pipelineFrameH = previousProc->GetOutFrameHeigth();
	        }

	        if (!_isPrepared) {    // for first time preparation
	            // initialize current proc
	            numInitialized = (*it)->Initialize(pipelineFrameW, pipelineFrameH, pipelineFrameW, pipelineFrameH, num, num == 0 && inputFormat != GL_NONE);
	        } else {    // for reinitialization with different frame size
	            numInitialized = (*it)->Reinit(pipelineFrameW, pipelineFrameH, num == 0 && inputFormat != GL_NONE);
	        }

	        // if this proc will downscale, we should generate a mipmap for the previous output
	        if (num > 0) {
	            // create a texture that is attached to an FBO for the output
	        	previousProc->InitFrameBufferTexture();
	        }

	        // set pointer to previous proc
	        previousProc = *it;

	        num += numInitialized;
	    }

	    // create the FBO texture for the last processor, too
	    //if(_pipeline.size() > 1)
	    previousProc->InitFrameBufferTexture();

	    // concatenate all processors
	    previousProc = NULL;

	    for (std::list<BasicProcessor *>::iterator it = _pipeline.begin();
	         it != _pipeline.end();
	         ++it)
	    {
	        if (previousProc) {
	            // set input texture id
	            (*it)->UseTexture(previousProc->GetOutputTextureId());  // previous output is this proc's input
	        }

	        // set pointer to previous proc
	        previousProc = *it;
	    }

	    // set last processor
	    _lastProcessor = previousProc;

	    // get input texture id
	    _inputTextureId = _firstProcessor->GetInputTextureId();

	    // set output texture id and size
	    _outputTextureId = _lastProcessor->GetOutputTextureId();
	    _oFrameWidth = _lastProcessor->GetOutFrameWidth();
	    _oFrameHeight = _lastProcessor->GetOutFrameHeigth();

	    // initialize render display if necessary
	    if (_renderDisp) {
	        if (!_isPrepared) {
	        	_renderDisp->Initialize(_oFrameWidth, _oFrameHeight, _oFrameWidth, _oFrameHeight, num, false);
	        } else {
	        	_renderDisp->Reinit(_oFrameWidth, _oFrameHeight);
	        }

	        _renderDisp->UseTexture(_outputTextureId);
	    }

	    printf(" prepared (input tex %d, output tex %d)",  _inputTextureId, _outputTextureId);

	    // print report (to spot errors in the pipeline)
//	    for (std::list<BasicProcessor *>::iterator it = _pipeline.begin();
//	         it != _pipeline.end();
//	         ++it)
//	    {
//	        (*it)->PrintInfo();
//	    }

	    glFinish();

	    _isPrepared = true;
}

MemoryTransfer* raspi_gpgpu::Controller::GetInMemoryTransfer() const {
	 return (_firstProcessor->GetMemoryTransferObject());
}

MemoryTransfer* raspi_gpgpu::Controller::GetOutMemoryTransfer() const {
	return (_lastProcessor->GetMemoryTransferObject());
}

void raspi_gpgpu::Controller::SetInputTextureId(GLuint inputTextureId,
		GLenum inputTextureTarget) {
	_inputTextureId = inputTextureId;
	_inputTextureTarget = inputTextureTarget;
	_firstProcessor->UseTexture(_inputTextureId, 1, _inputTextureTarget);
}

void raspi_gpgpu::Controller::SetInputData(const unsigned char* source) {
	assert(_isInitialized && _inputTextureId > 0);
	glActiveTexture(GL_TEXTURE1);
	_firstProcessor->SetExternalInputData(source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	Tool::LastGLError("raspi_gpgpu::Controller::SetInputData", "Texture params did not set");
	glFinish();
}

void raspi_gpgpu::Controller::Process() {
	  assert(_isInitialized);
	  _firstProcessor->UseTexture(_inputTextureId, 1, _inputTextureTarget);

	    for (std::list<BasicProcessor *>::iterator it = _pipeline.begin();
	         it != _pipeline.end();
	         ++it)
	    {
	        (*it)->DoRender();
	        glFinish();
	    }

}

void raspi_gpgpu::Controller::GetOutputData(unsigned char* buffer) {
	assert(_isInitialized);
	glFinish();
	_lastProcessor->GetResultData(buffer);

}



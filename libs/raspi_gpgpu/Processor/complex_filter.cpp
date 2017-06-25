
/*
 * complex_filter.cpp
 *
 *  Created on: 21 sty 2016
 *      Author: gregStudent
 */


#include "complex_filter.h"


raspi_gpgpu::ComplexFilter::~ComplexFilter() {
	for (std::list<BasicProcessor *>::iterator it = _processorPipeline.begin();
	         it != _processorPipeline.end();
	         ++it)
	    {
	        delete *it;
	    }

	_processorPipeline.clear();
}

void raspi_gpgpu::ComplexFilter::Dispose() {
	for (std::list<BasicProcessor *>::iterator it = _processorPipeline.begin();
		         it != _processorPipeline.end();
		         ++it)
		    {
		        (*it)->Dispose();
		    }
}

int raspi_gpgpu::ComplexFilter::Initialize(int inputWidth, int inputHeight,
		int outputWidth, int outputHeight, unsigned int order,
		bool externalInput) {

		BasicProcessor *pProc = NULL;
	    int num = 0;
	    int numInitialized = 0;
	    for (std::list<BasicProcessor *>::iterator it = _processorPipeline.begin();
	         it != _processorPipeline.end();
	         ++it)
	    {
	        int tFrameW, tFrameH;

	        if (num == 0) {
	         	tFrameW = inputWidth;
	        	tFrameH = inputHeight;
	        } else {
	          tFrameW = pProc->GetOutFrameWidth();
	          tFrameH = pProc->GetOutFrameHeigth();
	        }

	        numInitialized = (*it)->Initialize(tFrameW, tFrameH, tFrameW, tFrameH, num, num == 0 ? externalInput : false);
	        pProc = *it;
	        num += numInitialized;
	    }

	    return num;
}

int raspi_gpgpu::ComplexFilter::Reinit(int inputWidth, int inputHeight,
		bool externalInput) {

		BasicProcessor *pProc = NULL;
		int num = 0;
		int numInitialized = 0;
		for (std::list<BasicProcessor *>::iterator it = _processorPipeline.begin();
			 it != _processorPipeline.end();
			 ++it)
		{
			int tFrameW, tFrameH;

			if (num == 0) {
				tFrameW = inputWidth;
				tFrameH = inputHeight;
			} else {
			  tFrameW = pProc->GetOutFrameWidth();
			  tFrameH = pProc->GetOutFrameHeigth();
			}

			numInitialized = (*it)->Reinit(tFrameW, tFrameH,  num == 0 ? externalInput : false);
			pProc = *it;
			num += numInitialized;
		}

		return num;
}

void raspi_gpgpu::ComplexFilter::SetExternalInputFormat(GLenum format) {
	assert(_firstProcessor);
	_firstProcessor->SetExternalInputFormat(format);
}

void raspi_gpgpu::ComplexFilter::SetExternalInputData(
		const unsigned char* source) {
	 assert(_firstProcessor);
	 _firstProcessor->SetExternalInputData(source);
}

void raspi_gpgpu::ComplexFilter::InitFrameBufferTexture() {
	for (std::list<BasicProcessor *>::iterator it = _processorPipeline.begin();
	         it != _processorPipeline.end();
	         ++it)
	    {
	        (*it)->InitFrameBufferTexture();
	    }
}

void raspi_gpgpu::ComplexFilter::SetOutputSize(int outputWidth,
		int outputHeight) {
	 assert(_firstProcessor);
	 _firstProcessor->SetOutputSize(outputWidth, outputHeight);
}

//void raspi_gpgpu::ComplexFilter::SetOutputRenderOrientation(int orientation) {
//}

int raspi_gpgpu::ComplexFilter::GetOutFrameWidth() const {
	assert(_lastProcessor);
	return (_lastProcessor->GetOutFrameWidth());
}

int raspi_gpgpu::ComplexFilter::GetOutFrameHeigth() const {
	assert(_lastProcessor);
	return (_lastProcessor->GetOutFrameHeigth());
}

void raspi_gpgpu::ComplexFilter::GetResultData(unsigned char* source) const {
	 assert(_lastProcessor);
	 return (_lastProcessor->GetResultData(source));
}

raspi_gpgpu::MemoryTransfer* raspi_gpgpu::ComplexFilter::GetMemoryTransferObject() const {
	 assert(_lastProcessor);
	 return (_lastProcessor->GetMemoryTransferObject());
}

GLuint raspi_gpgpu::ComplexFilter::GetInputTextureId() const {
	assert(_firstProcessor);
	return (_firstProcessor->GetInputTextureId());
}

GLuint raspi_gpgpu::ComplexFilter::GetOutputTextureId() const {
	assert(_lastProcessor);
	return (_lastProcessor->GetOutputTextureId());
}

void raspi_gpgpu::ComplexFilter::UseTexture(GLuint id, GLuint useTexUnit,
		GLenum target) {
	BasicProcessor *prevProc = NULL;
	for (std::list<BasicProcessor *>::iterator it = _processorPipeline.begin();
		 it != _processorPipeline.end();
		 ++it)
	{
		if (!prevProc) {
			(*it)->UseTexture(id, useTexUnit, target);
		} else {
			(*it)->UseTexture(prevProc->GetOutputTextureId(), prevProc->GetTextureUnit());
		}

		prevProc = *it;
	}
}

GLuint raspi_gpgpu::ComplexFilter::GetTextureUnit() {
	 assert(_firstProcessor);
	 return (_firstProcessor->GetTextureUnit());
}

void raspi_gpgpu::ComplexFilter::DoRender() {
	for (std::list<BasicProcessor *>::iterator it = _processorPipeline.begin();
	         it != _processorPipeline.end();
	         ++it)
	    {
			(*it)->ResetProcVariables();
	        (*it)->DoRender();
	    }
}

void raspi_gpgpu::ComplexFilter::ComplexInit() {
	_firstProcessor = _processorPipeline.front();
	_lastProcessor = _processorPipeline.back();
}









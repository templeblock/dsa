/*
 * basic_processor.cpp
 *
 *  Created on: 11 sty 2016
 *      Author: gregStudent
 */

#include "basic_processor.h"
#include <string>


  const GLfloat raspi_gpgpu::BasicProcessor::TexCoordArray[] = {
			    0, 0,
			    1, 0,
			    0, 1,
			    1, 1 };
  const GLfloat raspi_gpgpu::BasicProcessor::TexCoordDiagonalArray[]{
		    0, 0,
		    0, 1,
		    1, 0,
		    1, 1 };

  GLfloat raspi_gpgpu::BasicProcessor::Vertices[] = {
		    -1, -1, 0,
		     1, -1, 0,
		    -1,  1, 0,
		     1,  1, 0 };

raspi_gpgpu::BasicProcessor::BasicProcessor() {
	_processorOutWidth = _processorOutHeight = 0;
	_outputFrameWidth = _outputFrameHeight = 0;
	_inTextureId = 0;
	_intextureUnit = 1;
	_shader = NULL;
	_frameBufferObject = NULL;
	_rOrientation = 0;
}

raspi_gpgpu::BasicProcessor::~BasicProcessor() {
	Dispose();
}

int raspi_gpgpu::BasicProcessor::Reinit(int inputWidth, int inputHeight,
	bool externalInput) {
	assert(_frameBufferObject != NULL);

	SetInOutFrameSizes(inputWidth, inputHeight, _processorOutWidth, _processorOutHeight);

	_frameBufferObject->DisposeOutputTex();
	if (externalInput) {
		UseTexture(_frameBufferObject->GetMemoryTransfer()->InitInput(_inFrameWidth, _inFrameHeight, _inputDataFormat));
	}
	std::cout <<(GetProcessorName(), "reinit  insize: %dx%d, outsize:  %dx%d", _inFrameWidth, _inFrameHeight, _outputFrameWidth, _outputFrameHeight) << std::endl;
	return 1;
}


void raspi_gpgpu::BasicProcessor::Dispose() {
	if (_frameBufferObject) {
			delete _frameBufferObject;
			_frameBufferObject = NULL;
			_outputFrameWidth = _outputFrameHeight = 0;
		}

	if (_shader) {
		delete _shader;
		_shader = NULL;
	}
}

int raspi_gpgpu::BasicProcessor::Initialize(int inputWidth, int inputHeight, int outputWidth, int outputHeight, unsigned int order, bool externalInput = false) {
	// to samo co base init
	assert(inputWidth > 0 && inputHeight > 0);

	_order = order;
	SetInOutFrameSizes(inputWidth, inputHeight, outputWidth, outputHeight);

	if (externalInput) {
		assert(_frameBufferObject != NULL);
		UseTexture(_frameBufferObject->GetMemoryTransfer()->InitInput(inputWidth, inputHeight, _inputDataFormat));
		std::cout <<(GetProcessorName(), "Input ready") << std::endl;
	}

	std::cout <<GetProcessorName() << std::endl;
	printf( "IN size (%dx%d), OUT (size %dx%d)",
	_inFrameWidth, _inFrameHeight, _outputFrameWidth, _outputFrameHeight);
	std::cout  << std::endl;
}


void raspi_gpgpu::BasicProcessor::SetExternalInputData(
		const unsigned char* source) {
	_frameBufferObject->GetMemoryTransfer()->TransferToGPU(source);
}


void raspi_gpgpu::BasicProcessor::InitFrameBufferTexture() {
	assert(_frameBufferObject != NULL);
	_frameBufferObject->CreateOutputTexture(_outputFrameWidth, _outputFrameHeight);
	_outputFrameWidth = _frameBufferObject->GetTextureWidth();
	_outputFrameHeight = _frameBufferObject->GetTextureHeight();
}





void raspi_gpgpu::BasicProcessor::GetResultData(unsigned char* source) const {
	 assert(_frameBufferObject != NULL);
	 _frameBufferObject->ReadBuffer(source);
}


raspi_gpgpu::MemoryTransfer* raspi_gpgpu::BasicProcessor::GetMemoryTransferObject() const {
	assert(_frameBufferObject);
    return (_frameBufferObject->GetMemoryTransfer());
}

GLuint raspi_gpgpu::BasicProcessor::GetOutputTextureId() const {
	 assert(_frameBufferObject != NULL);
	 return _frameBufferObject->GetOutputTextureId();
}


void raspi_gpgpu::BasicProcessor::SetInOutFrameSizes(int inputWidth, int inputHeight, int outputWidth, int outputHeight) {
	_inFrameWidth = inputWidth;
	_inFrameHeight = inputHeight;
	assert(outputWidth > 0 && outputHeight > 0);
	_outputFrameWidth = outputWidth;
	_outputFrameHeight = outputHeight;
}


void raspi_gpgpu::BasicProcessor::CreateFBO() {
	assert(_frameBufferObject == NULL);
	_frameBufferObject = new FrameBufferManager();
	_frameBufferObject->SetTextureUnit(1);
}


void raspi_gpgpu::BasicProcessor::PrintInit() {
	 assert(_frameBufferObject);

	 std::cout <<(GetProcessorName(), " Order: %d, inTex %d [%dx%d], outTex %d [%dx%d]",
	    		_order,	_inTextureId, _inFrameWidth, _inFrameHeight,	_frameBufferObject->GetOutputTextureId(), _outputFrameWidth, _outputFrameHeight) << std::endl;
}

void raspi_gpgpu::BasicProcessor::InitShader(const char* vertexSource,
	 const char* fSource, GLenum target) {
	if (_shader) {
		if (_inTextureTarget != target)
			delete (_shader);
		else
			return;
	}


		std::string fragmentSource(fSource);
		_shader = new ShaderFactory();
		assert(_shader->Build(fragmentSource.c_str(), vertexSource));
		std::cout <<(GetProcessorName(), "raspi_gpgpu::BasicProcessor::InitShader OK") << std::endl;

}







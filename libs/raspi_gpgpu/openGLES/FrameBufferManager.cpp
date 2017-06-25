/*
 * FrameBufferManager.cpp
 *
 *  Created on: 11 sty 2016
 *      Author: gregStudent
 */

#include "FrameBufferManager.h"
#include "../Processor/Controller.cpp"


raspi_gpgpu::FrameBufferManager::FrameBufferManager() {
	_id = 0;
	_tWidth = _tHeight = 0;
	_outputTextureId = 0;
	_textureUnit = 0;
	_core = raspi_gpgpu::Controller::GetInstance();
	_mem = MemoryTransfer::Instance();
	_mem->Initialize();

	CreateID();
}

raspi_gpgpu::FrameBufferManager::~FrameBufferManager() {
	DisposeFBO();
	delete _mem;
}

void raspi_gpgpu::FrameBufferManager::CreateOutputTexture(int width, int height,
		GLenum attachment) {

   assert(_mem && width > 0 && height > 0);
   _tWidth = width;
   _tHeight= height;


	Bind();

	// create attached texture
	glActiveTexture(GL_TEXTURE0 + _textureUnit);
	_outputTextureId = _mem->InitOutput(_tWidth, _tHeight);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER,
						   attachment,
						   GL_TEXTURE_2D,
						   _outputTextureId, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("raspi_gpgpu::FrameBufferManager::CreateOutputTexture (error %d)", status);
		std::cout << std::endl;
		_outputTextureId = 0;
	} else {
		printf("raspi_gpgpu::FrameBufferManager::CreateOutputTexture %d: created  texture %d of size %d;%d",  _id, _outputTextureId, _tWidth, _tHeight);
		std::cout << std::endl;
	}
	Unbind();
}

void raspi_gpgpu::FrameBufferManager::DisposeOutputTex() {
	 assert(_mem);
     _mem->ClearOutput();
}

void raspi_gpgpu::FrameBufferManager::ReadBuffer(unsigned char* buffer) {
	assert(_mem && _outputTextureId > 0 &&  _tHeight > 0 && _tWidth  > 0);
	Bind();
	_mem->TransferFromGPU(buffer);
	Unbind();
}

void raspi_gpgpu::FrameBufferManager::DisposeFBO() {
	glDeleteFramebuffers(1, &_id);
}

void raspi_gpgpu::FrameBufferManager::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void raspi_gpgpu::FrameBufferManager::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void raspi_gpgpu::FrameBufferManager::CreateID() {
	 glGenFramebuffers(1, &_id);
}






/*
 * MemoryTransfer.cpp
 *
 *  Created on: 15 gru 2015
 *      Author: gregStudent
 */

#include "MemoryTransfer.h"

using namespace raspi_gpgpu;

MemoryTransfer::MemoryTransfer()
{
	_inputWidth = _inputHeight = _outputWidth = _outputHeight = 0;
	_inputTextureId = 0;
	_outputTextureId = 0;
	_initialized = false;
	_initializedInput = false;
	_initializedOutput = false;
	_inputFormat = GL_RGBA;
}

MemoryTransfer::~MemoryTransfer()
{
	this->ClearInput();
	this->ClearOutput();
}

void MemoryTransfer::ClearInput()
{
	 if (_inputTextureId > 0)
	 {
	        glDeleteTextures(1, &_inputTextureId);
	        _inputTextureId = 0;
	 }
}

void MemoryTransfer::ClearOutput()
{
	 if (_outputTextureId > 0) {
		        glDeleteTextures(1, &_outputTextureId);
		        _outputTextureId = 0;
		    }
}

void MemoryTransfer::InitCommonParameters(GLuint textureId) {
    if (textureId > 0) {
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

GLuint MemoryTransfer::InitInput(int textureWidth, int textureHeight, GLenum inputFormat = GL_RGBA)
{
	assert(this->_initialized && textureWidth > 0 && textureHeight > 0);

	if (this->_inputWidth == textureWidth && this->_inputHeight == textureHeight && this->_inputFormat == inputFormat) {
		return this->_inputTextureId; // es hat sich nichts veraendert
	}

	if (this->_initializedInput) {
		ClearInput();
	}

	this->_inputWidth = textureWidth;
	this->_inputHeight = textureHeight;
	this->_inputFormat = inputFormat;


	glGenTextures(1, &_inputTextureId);
	//GLenum err = glGetError();

	if (_inputTextureId == 0) {
		printf("MemoryTransfer", "Texture ID input konnte nicht erzeugt werden");
		return 0;
	}


	_initializedInput = true;

	return _inputTextureId;
}

GLuint MemoryTransfer::InitOutput(int textureWidth, int textureHeight)
{
	assert(this->_initialized && textureWidth > 0 && textureHeight > 0);

	if (this->_outputWidth == textureWidth && this->_outputHeight == textureHeight) {
		return this->_outputTextureId;
	}

	if (this->_initializedOutput) {
		ClearOutput();
	}

	this->_outputWidth = textureWidth;
	this->_outputHeight = textureHeight;

	glGenTextures(1, &this->_outputTextureId);

	if (_outputTextureId == 0) {
		printf("MemoryTransfer", "Texture ID output konnte nicht erzeugt werden");
		return 0;
	}

	InitCommonParameters(this->_outputTextureId);

	Tool::LastGLError("MemoryTransfer", "InitOutput");

	glTexImage2D(GL_TEXTURE_2D, 0,
			GL_RGB,
			textureWidth, textureHeight, 0,
				 GL_RGB, GL_UNSIGNED_BYTE,
				 NULL);

	Tool::LastGLError("MemTransfer", "glTexImage2D init");

	this->_initializedOutput = true;
	return this->_outputTextureId;
}

void MemoryTransfer::TransferToGPU(const unsigned char *buffer)
{
	assert(this->_initializedInput && this->_inputTextureId > 0 && buffer);

	glBindTexture(GL_TEXTURE_2D, _inputTextureId);
	//GLenum err = glGetError();
	try
	{
		//err = glGetError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->_inputWidth, this->_inputHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *) buffer); // Typ GL_RGBA funktioniert nicht
		//err = glGetError();
	}
	catch(const std::exception& e)
	{
		std::cout << "erro in try catch " << e.what() << std::endl;
	}

	// check for error
	Tool::LastGLError("MemoryTransfer", "TransferToGPU glTexImage2D");

	InitCommonParameters(0);
}

void MemoryTransfer::TransferFromGPU(unsigned char *buffer)
{

    assert(this->_initializedOutput && this->_outputTextureId > 0 && buffer);

    glBindTexture(GL_TEXTURE_2D, this->_outputTextureId);
	try
	{
		// default (and slow) way using glReadPixels:
		glReadPixels(0, 0, this->_outputWidth, raspi_gpgpu::RaspiConfig::GetHoughHeight(), GL_RGB, GL_UNSIGNED_BYTE, buffer);
		GLenum err = glGetError();
	}
	catch(const std::exception& e)
	{
		std::cout << "erro in MemTransfer::fromGPU " << e.what() << std::endl;
	}

    // check for error
    Tool::LastGLError("MemoryTransfer", "TransferFromGPU glReadPixels");
}

 MemoryTransfer* MemoryTransfer::Instance() {
	 MemoryTransfer *_instance = NULL;

	 if (!_instance) {
		_instance = new MemoryTransfer();

	    }

	    return (_instance);
}


/*
 * FrameBufferManager.h
 *
 *  Created on: 15 gru 2015
 *      Author: gregStudent
 */

#ifndef FRAMEBUFFERMANAGER_H_
#define FRAMEBUFFERMANAGER_H_

#include "../egl_include.h"
#include "MemoryTransfer.h"

namespace raspi_gpgpu
{

	class Controller;

	class FrameBufferManager {
	public:

		FrameBufferManager();


		virtual ~FrameBufferManager();

		int GetOutputTextureId() const { return _outputTextureId; }

		void SetTextureUnit(GLuint texUnit) { _textureUnit = texUnit; }

		GLuint GetTextureUnit() const { return _textureUnit; }

		void SetId(GLuint id) { _id = id; }

		GLuint GetId() const { return _id; }


		virtual void CreateOutputTexture(int width, int height, GLenum attachment = GL_COLOR_ATTACHMENT0);

		virtual void DisposeOutputTex();

		virtual void ReadBuffer(unsigned char *buf);

		virtual void DisposeFBO();


		int GetTextureWidth() const { return _tWidth; }

		int GetTextureHeight() const { return _tHeight; }

		MemoryTransfer *GetMemoryTransfer() const { return (_mem); }

		void Unbind();

		void Bind();


	protected:

		virtual void CreateID();


		Controller *_core;

		MemoryTransfer *_mem;

		GLuint _id;
		GLuint _textureUnit;
		GLuint _outputTextureId;

		int _tWidth;
		int _tHeight;
	};
}


#endif /* FRAMEBUFFERMANAGER_H_ */

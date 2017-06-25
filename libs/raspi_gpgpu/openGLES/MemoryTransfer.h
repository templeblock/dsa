/*
 * MemoryTransfer.h
 *
 *  Created on: 15 gru 2015
 *      Author: gregStudent
 */

#ifndef MEMORYTRANSFER_H_
#define MEMORYTRANSFER_H_


#include "../egl_include.h"
#include "../raspi_config.h"

namespace raspi_gpgpu
{
  class MemoryTransfer
  {
  public:
	  MemoryTransfer();
	  static MemoryTransfer *Instance();

	  ~MemoryTransfer();

	  //void Init(){};
	  GLuint InitInput (int textureWidth, int textureHeight, GLenum inputFormat);
	  GLuint InitOutput(int textureWidth, int textureHeight);
	  void Initialize() {_initialized = true;}

	  GLuint getInputTexId()  { return (_inputTextureId); }
	  GLuint getOutputTexId()  { return (_outputTextureId); }

	  void ClearInput();
	  void ClearOutput();

	  void TransferToGPU(const unsigned char *buf);
	  void TransferFromGPU(unsigned char *buf);

	  void InitCommonParameters(GLuint texId);

  protected:
	     bool _initialized;
	     bool _initializedInput;
	     bool _initializedOutput;


	     int _inputWidth;             // input texture width
	     int _inputHeight;             // input texture height
	     int _outputWidth;            // output texture width
	     int _outputHeight;            // output texture heights

	     GLuint _inputTextureId;      // input texture id
	     GLuint _outputTextureId;     // output texture id

	     GLenum _inputFormat;    // input texture pixel forma
  };
}

#endif /* MEMORYTRANSFER_H_ */

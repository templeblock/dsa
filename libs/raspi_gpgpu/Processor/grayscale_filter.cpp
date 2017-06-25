/*
 * grayscale_filter.cpp
 *
 *  Created on: 12 sty 2016
 *      Author: gregStudent
 */

#include "grayscale_filter.h"
#include <stdio.h>

const GLfloat raspi_gpgpu::GrayscaleProcessor::_grayscaleConvVecRGB[3] = {
    0.299, 0.587, 0.114
};

const char *raspi_gpgpu::GrayscaleProcessor::_fragmentShaderSource =  STRINGIFY(
			precision mediump float;
			varying vec2 vTexCoord;
			uniform sampler2D uInputTex;
			uniform vec3 uInputConvVec;
			float threshold = 0.7166666; //0.5;
			void main() {
				vec4 pixel = texture2D(uInputTex, vTexCoord);
				if(pixel.r > threshold)
					gl_FragColor = vec4(0.7031, 0.7031, 0.7031, 1.0);
				else
              gl_FragColor = pixel;
			}
		);



raspi_gpgpu::GrayscaleProcessor::GrayscaleProcessor() {
	const GLfloat *x =  &_grayscaleConvVecRGB[0];
	memcpy(_grayscaleConversion, x, sizeof(GLfloat) * 3);
}



int raspi_gpgpu::GrayscaleProcessor::Initialize(int inputWidth, int inputHeight,
	int outputWidth, int outputHeight, unsigned int order,
	bool externalInput) {

	std::cout<< GetProcName() << " Raspi_gpgpu::GrayscaleProcessor::Initialize" << std::endl;
	this->CreateFBO();
	BasicProcessor::Initialize(inputWidth, inputHeight, outputWidth, outputHeight, order, externalInput);
	InitFilter(_fragmentShaderSource);
	_shaderParameter = _shader->GetParameter(1, "uInputConvVec");
	return (1);
}


void raspi_gpgpu::GrayscaleProcessor::DoRender() {
	printf("%s INtex %d, target %d, fbo %dx%d", GetProcName(), _inTextureId, _inTextureTarget, _outputFrameWidth, _outputFrameHeight);

	RenderPreinit();
	glUniform3fv(_shaderParameter, 1, _grayscaleConversion);        // set additional uniforms
	Tool::LastGLError(GetProcName(), "GrayscaleProcessor RenderPreinit");

	RenderSetCoordinates();
	Tool::LastGLError(GetProcName(), "GrayscaleProcessor RenderSetCoordinates");

	RenderDraw();
	Tool::LastGLError(GetProcName(), "GrayscaleProcessor RenderDraw");

	RenderDispose();
	Tool::LastGLError(GetProcName(), "GrayscaleProcessor RenderDispose");
}






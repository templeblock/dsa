/*
 * adaptive_threshold.cpp
 *
 *  Created on: 19 sty 2016
 *      Author: gregStudent
 */


#include "adaptive_threshold.h"

 const char *raspi_gpgpu::AdaptiveThreshold::_fragmentAdaptThresh1Src = STRINGIFY(
precision mediump float;
varying vec2 vTexCoord;
uniform vec2 uPxD;
uniform sampler2D uInputTex;
void main() {
    // get center pixel value
    float centerGray = texture2D(uInputTex, vTexCoord).r;
    // get the sum
    float sum = texture2D(uInputTex, vTexCoord + vec2(uPxD.x * -2.0, 0.0)).r +
    texture2D(uInputTex, vTexCoord + vec2(uPxD.x * -1.0, 0.0)).r +
    centerGray +
    texture2D(uInputTex, vTexCoord + vec2(uPxD.x *  1.0, 0.0)).r +
    texture2D(uInputTex, vTexCoord + vec2(uPxD.x *  2.0, 0.0)).r;
    // get the average
    float avg = sum / 5.0;
    // Result stores average pixel value (R) and original gray value (G)
    gl_FragColor = vec4(avg, centerGray, 0.0, 1.0);
}
);


const char *raspi_gpgpu::AdaptiveThreshold::_fragmentAdaptThresh2Src = STRINGIFY(
precision mediump float;
varying vec2 vTexCoord;
uniform vec2 uPxD;
uniform sampler2D uInputTex;
void main() {
    vec4 centerPx = texture2D(uInputTex, vTexCoord); // stores: horizontal avg, orig. gray value, 0, 1
    const float bigC = 3.1 / 255.0;  // 9.5
    // get the sum
    float sum = texture2D(uInputTex, vTexCoord + vec2(uPxD.y * -2.0, 0.0)).r +
    texture2D(uInputTex, vTexCoord + vec2(uPxD.y * -1.0, 0.0)).r +
    centerPx.r +
    texture2D(uInputTex, vTexCoord + vec2(uPxD.y *  1.0, 0.0)).r +
    texture2D(uInputTex, vTexCoord + vec2(uPxD.y *  2.0, 0.0)).r;
    // get the average
    float avg = sum / 5.0;
    // create inverted binary value
    float bin = 1.0 - step(avg - bigC, centerPx.g); // centerPx.g is orig. gray value at current position
    // store thresholded values
    gl_FragColor = vec4(bin, bin, bin, 1.0);
}
);

int raspi_gpgpu::AdaptiveThreshold::Initialize(int inputWidth, int inputHeight,
		int outputWidth, int outputHeight, unsigned int order,
		bool externalInput) {

	    printf("%s", GetProcName());
	    printf( "render pass %d", _pass);

	    this->CreateFBO();

	    BasicProcessor::Initialize(inputWidth, inputHeight, outputWidth, outputHeight, order, externalInput);


	    _Dx = 1.0f / (float)outputWidth;
	    _Dy = 1.0f / (float)outputHeight;


	    const char *source = _pass == 1 ? _fragmentAdaptThresh1Src : _fragmentAdaptThresh2Src;
	    InitFilter(source,1);

	    _shParameterd = _shader->GetParameter(1, "uPxD");

	    return 1;

}

void raspi_gpgpu::AdaptiveThreshold::DoRender() {
	printf("%s INtex %d, target %d, fbo %dx%d", GetProcName(), _inTextureId, _inTextureTarget, _outputFrameWidth, _outputFrameHeight);

		RenderPreinit();
		glUniform2f(_shParameterd, _Dx, _Dy);

		Tool::LastGLError(GetProcName(), "AdaptiveThreshold RenderPreinit");

		RenderSetCoordinates();
		Tool::LastGLError(GetProcName(), "AdaptiveThreshold RenderSetCoordinates");

		RenderDraw();
		Tool::LastGLError(GetProcName(), "AdaptiveThreshold RenderDraw");

		RenderDispose();
		Tool::LastGLError(GetProcName(), "AdaptiveThreshold RenderDispose");
}

void raspi_gpgpu::AdaptiveThreshold::InitFrameBufferTexture() {
	 assert(_frameBufferObject);

	    if (_pass == 1) {
	    	_frameBufferObject->CreateOutputTexture(_outputFrameHeight, _outputFrameWidth);
	    } else {
	    	_frameBufferObject->CreateOutputTexture(_outputFrameWidth, _outputFrameHeight);
	    }

	    _outputFrameWidth = _frameBufferObject->GetTextureWidth();
	    _outputFrameHeight= _frameBufferObject->GetTextureHeight();
}






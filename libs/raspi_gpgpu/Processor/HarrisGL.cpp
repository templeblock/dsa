/*
 * HarrisGL.cpp
 *
 *  Created on: 23 lut 2016
 *      Author: gregStudent
 */

#include "HarrisGL.h"

// XY derivative
const char *raspi_gpgpu::HarrisGL::_fragmentHarris1Src = STRINGIFY(
	precision mediump float;
	varying vec2 vTexCoord;
	uniform sampler2D uInputTex;


	 float texelWidth = 1.0 / 1280.0;
	 float edgeStrength = 1.0;
	 float texelHeight = 1.0 / 768.0;

	 vec2 widthStep = vec2(texelWidth, 0.0);
	 vec2 heightStep = vec2(0.0, texelHeight);
	 vec2 widthHeightStep = vec2(texelWidth, texelHeight);
	 vec2 widthNegativeHeightStep = vec2(texelWidth, - texelHeight);

	 vec2 leftTextureCoordinate = vTexCoord.xy - widthStep;
	 vec2 rightTextureCoordinate = vTexCoord.xy + widthStep;

	 vec2 topTextureCoordinate = vTexCoord.xy - heightStep;
	 vec2 topLeftTextureCoordinate = vTexCoord.xy - widthHeightStep;
	 vec2 topRightTextureCoordinate = vTexCoord.xy + widthNegativeHeightStep;

	 vec2 bottomTextureCoordinate = vTexCoord.xy + heightStep;
	 vec2 bottomLeftTextureCoordinate = vTexCoord.xy - widthNegativeHeightStep;
	 vec2 bottomRightTextureCoordinate = vTexCoord.xy + widthHeightStep;


	 void main()
	 {
	     float topIntensity = texture2D(uInputTex, topTextureCoordinate).r;
	     float topRightIntensity = texture2D(uInputTex, topRightTextureCoordinate).r;
	     float topLeftIntensity = texture2D(uInputTex, topLeftTextureCoordinate).r;
	     float bottomIntensity = texture2D(uInputTex, bottomTextureCoordinate).r;
	     float bottomLeftIntensity = texture2D(uInputTex, bottomLeftTextureCoordinate).r;
	     float bottomRightIntensity = texture2D(uInputTex, bottomRightTextureCoordinate).r;
	     float leftIntensity = texture2D(uInputTex, leftTextureCoordinate).r;
	     float rightIntensity = texture2D(uInputTex, rightTextureCoordinate).r;

	     float verticalDerivative = -topLeftIntensity - topIntensity - topRightIntensity + bottomLeftIntensity + bottomIntensity + bottomRightIntensity;
	     float horizontalDerivative = -bottomLeftIntensity - leftIntensity - topLeftIntensity + bottomRightIntensity + rightIntensity + topRightIntensity;
	     verticalDerivative = verticalDerivative * edgeStrength;
	     horizontalDerivative = horizontalDerivative * edgeStrength;

	     // Scaling the X * Y operation so that negative numbers are not clipped in the 0..1 range. This will be expanded in the corner detection filter
	     gl_FragColor = vec4(horizontalDerivative * horizontalDerivative, verticalDerivative * verticalDerivative, ((verticalDerivative * horizontalDerivative) + 1.0) / 2.0, 1.0);
	 }
);

const char *raspi_gpgpu::HarrisGL::_fragmentHarris2Src = STRINGIFY(

		precision mediump float;
		varying vec2 vTexCoord;
		uniform sampler2D uInputTex;
		vec3 offset = vec3( 0.0, 1.3846153846, 3.2307692308 );
	   vec3 weight = vec3( 0.2270270270, 0.3162162162, 0.0702702703 );

		void main() {
			vec3 tmp = texture2D(uInputTex, vTexCoord).rgb * weight[0];

			for (int i=1; i<3; i++)
			{
				tmp += texture2D(uInputTex, vTexCoord + vec2(0.0, offset[i])/ 768.0).rgb  * weight[i];
				tmp += texture2D(uInputTex, vTexCoord - vec2(0.0, offset[i])/ 768.0).rgb  * weight[i];
			}

			gl_FragColor = vec4(tmp.r, tmp.g, tmp.b, 1.0);
		}

);

const char *raspi_gpgpu::HarrisGL::_fragmentHarris3Src = STRINGIFY(
		precision mediump float;
		varying vec2 vTexCoord;
		uniform sampler2D uInputTex;
		vec3 offset = vec3( 0.0, 1.3846153846, 3.2307692308 );
	   vec3 weight = vec3( 0.2270270270, 0.3162162162, 0.0702702703 );

		void main() {
			vec3 tmp = texture2D(uInputTex, vTexCoord).rgb * weight[0];

			for (int i=1; i<3; i++)
			{
				tmp += texture2D(uInputTex, vTexCoord + vec2(offset[i])/ 1280.0, 0.0).rgb  * weight[i];
				tmp += texture2D(uInputTex, vTexCoord - vec2(offset[i])/ 1280.0, 0.0).rgb  * weight[i];
			}

			gl_FragColor = vec4(tmp.r, tmp.g, tmp.b, 1.0);
		}
);

const char *raspi_gpgpu::HarrisGL::_fragmentHarris4Src = STRINGIFY(

		//self.blurRadiusInPixels = 2.0;
		//self.sensitivity = 5.0;
		//self.threshold = 0.20;
		// harrisConstant = 0.04;

		varying vec2 vTexCoord;
		uniform sampler2D uInputTex;
		float sensitivity = 1.0;
		float harrisConstant = 0.0001;

		 void main()
		 {
		     vec3 derivativeElements = texture2D(uInputTex, vTexCoord).rgb;
		     float derivativeSum = derivativeElements.x + derivativeElements.y;
		     float zElement = (derivativeElements.z * 2.0) - 1.0;
		     // R = Ix^2 * Iy^2 - Ixy * Ixy - k * (Ix^2 + Iy^2)^2
		     float cornerness = derivativeElements.x * derivativeElements.y - (zElement * zElement) - harrisConstant * derivativeSum * derivativeSum;

		     gl_FragColor = vec4(vec3(cornerness * sensitivity), 1.0);
		 }

);


const char *raspi_gpgpu::HarrisGL::_fragmentHarris5Src = STRINGIFY(
		 precision mediump float;
				 varying vec2 vTexCoord;
				 uniform sampler2D uInputTex;
				 float texelWidth = 1.0 / 1280.0;
				 float texelHeight = 1.0 / 768.0;
				 float threshold = 0.01;

				 vec2 widthStep = vec2(texelWidth, 0.0);
				 vec2 heightStep = vec2(0.0, texelHeight);
				 vec2 widthHeightStep = vec2(texelWidth, texelHeight);
				 vec2 widthNegativeHeightStep = vec2(texelWidth, - texelHeight);

				 vec2 leftTextureCoordinate = vTexCoord.xy - widthStep;
				 vec2 rightTextureCoordinate = vTexCoord.xy + widthStep;

				 vec2 topTextureCoordinate = vTexCoord.xy - heightStep;
				 vec2 topLeftTextureCoordinate = vTexCoord.xy - widthHeightStep;
				 vec2 topRightTextureCoordinate = vTexCoord.xy + widthNegativeHeightStep;

				 vec2 bottomTextureCoordinate = vTexCoord.xy + heightStep;
				 vec2 bottomLeftTextureCoordinate = vTexCoord.xy - widthNegativeHeightStep;
				 vec2 bottomRightTextureCoordinate = vTexCoord.xy + widthHeightStep;


				 void main()
				 {
					  float bottomColor = texture2D(uInputTex, bottomTextureCoordinate).r;
					  float bottomLeftColor = texture2D(uInputTex, bottomLeftTextureCoordinate).r;
					  float bottomRightColor = texture2D(uInputTex, bottomRightTextureCoordinate).r;
					  vec4 centerColor = texture2D(uInputTex, vTexCoord);
					  float leftColor = texture2D(uInputTex, leftTextureCoordinate).r;
					  float rightColor = texture2D(uInputTex, rightTextureCoordinate).r;
					  float topColor = texture2D(uInputTex, topTextureCoordinate).r;
					  float topRightColor = texture2D(uInputTex, topRightTextureCoordinate).r;
					  float topLeftColor = texture2D(uInputTex, topLeftTextureCoordinate).r;

					  // Use a tiebreaker for pixels to the left and immediately above this one
					  float multiplier = 1.0 - step(centerColor.r, topColor);
					  multiplier = multiplier * (1.0 - step(centerColor.r, topLeftColor));
					  multiplier = multiplier * (1.0 - step(centerColor.r, leftColor));
					  multiplier = multiplier * (1.0 - step(centerColor.r, bottomLeftColor));

					  float maxValue = max(centerColor.r, bottomColor);
					  maxValue = max(maxValue, bottomRightColor);
					  maxValue = max(maxValue, rightColor);
					  maxValue = max(maxValue, topRightColor);

					  float finalValue = centerColor.r * step(maxValue, centerColor.r) * multiplier;
					  finalValue = step(threshold, finalValue);

					  gl_FragColor = vec4(finalValue, finalValue, finalValue, 1.0);
				 }
);

int raspi_gpgpu::HarrisGL::Initialize(int inputWidth, int inputHeight,
		int outputWidth, int outputHeight, unsigned int order,
		bool externalInput) {

	printf("%s", GetProcName());
		printf( "render pass %d", _pass);

		this->CreateFBO();

		BasicProcessor::Initialize(inputWidth, inputHeight, outputWidth, outputHeight, order, externalInput);

		const char *source;
		if(_pass == 1)
			source =  _fragmentHarris1Src;
		else if(_pass == 2)
			source =  _fragmentHarris2Src;
		else if(_pass == 3)
			source =  _fragmentHarris3Src;
		else if(_pass == 4)
			source =  _fragmentHarris4Src;
		else if(_pass == 5)
			source =  _fragmentHarris5Src;


		InitFilter(source,0);
		return 1;

}

void raspi_gpgpu::HarrisGL::DoRender() {

	RenderPreinit();
	Tool::LastGLError(GetProcName(), "HarrisGL RenderPreinit");

	Tool::LastGLError(GetProcName(), "HarrisGL passing uniforms");

	RenderSetCoordinates();
	Tool::LastGLError(GetProcName(), "HarrisGL RenderSetCoordinates");

	RenderDraw();
	Tool::LastGLError(GetProcName(), "HarrisGL RenderDraw");

	RenderDispose();
	Tool::LastGLError(GetProcName(), "HarrisGL RenderDispose");

}

void raspi_gpgpu::HarrisGL::InitFrameBufferTexture() {
	assert(_frameBufferObject);
	_frameBufferObject->CreateOutputTexture(_outputFrameWidth, _outputFrameHeight);
	_outputFrameWidth = _frameBufferObject->GetTextureWidth();
	_outputFrameHeight= _frameBufferObject->GetTextureHeight();
}





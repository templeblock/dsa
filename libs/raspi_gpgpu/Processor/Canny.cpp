/*
 * Canny.cpp
 *
 *  Created on: 30 sty 2016
 *      Author: gregStudent
 */
#include "Canny.h"

// vertical blur
const char *raspi_gpgpu::Canny::_fragmentCanny1Src = STRINGIFY(
	precision mediump float;
	//uniform vec2 step;
	varying vec2 vTexCoord;
	uniform sampler2D uInputTex;
	uniform float iheight;

	vec3 offset = vec3( 0.0, 1.3846153846, 3.2307692308 );
	vec3 weight = vec3( 0.2270270270, 0.3162162162, 0.0702702703 );

	void main() {
		vec3 tmp = texture2D(uInputTex, vTexCoord).rgb * weight[0];

	    for (int i=1; i<3; i++)
	    {
	    	tmp += texture2D(uInputTex, vTexCoord + vec2(0.0, offset[i])/ iheight).rgb  * weight[i];
	    	tmp += texture2D(uInputTex, vTexCoord - vec2(0.0, offset[i])/ iheight).rgb  * weight[i];
	    }

		gl_FragColor = vec4(tmp.r, tmp.g, tmp.b, 1.0);
	}
);

// horizontal blur
const char *raspi_gpgpu::Canny::_fragmentCanny2Src = STRINGIFY(
		precision mediump float;
		uniform float iwidth;

		varying vec2 vTexCoord;
		uniform sampler2D uInputTex;
		vec3 offset = vec3( 0.0, 1.3846153846, 3.2307692308 );
			vec3 weight = vec3( 0.2270270270, 0.3162162162, 0.0702702703 );

		void main() {
			vec3 tmp = texture2D(uInputTex, vTexCoord).rgb * weight[0];

			for (int i=1; i<3; i++)
			{
				tmp += texture2D(uInputTex, vTexCoord + vec2(offset[i])/ iwidth, 0.0).rgb  * weight[i];
				tmp += texture2D(uInputTex, vTexCoord - vec2(offset[i])/ iwidth, 0.0).rgb  * weight[i];
			}

			gl_FragColor = vec4(tmp.r, tmp.g, tmp.b, 1.0);
		}
);


const char *raspi_gpgpu::Canny::_fragmentCanny3Src = STRINGIFY(
		 precision mediump float;
		 uniform float iwidth;
		 uniform float iheight;
		 varying vec2 vTexCoord;
		 uniform sampler2D uInputTex;

		 float texelWidth = 1.0 / iwidth;
		 float texelHeight = 1.0 / iheight;

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




		void main() {

			 float bottomLeftIntensity = texture2D(uInputTex, bottomLeftTextureCoordinate).r;
			 float topRightIntensity = texture2D(uInputTex, topRightTextureCoordinate).r;
			 float topLeftIntensity = texture2D(uInputTex, topLeftTextureCoordinate).r;
			 float bottomRightIntensity = texture2D(uInputTex, bottomRightTextureCoordinate).r;
			 float leftIntensity = texture2D(uInputTex, leftTextureCoordinate).r;
			 float rightIntensity = texture2D(uInputTex, rightTextureCoordinate).r;
			 float bottomIntensity = texture2D(uInputTex, bottomTextureCoordinate).r;
			 float topIntensity = texture2D(uInputTex, topTextureCoordinate).r;

			 vec2 gradientDirection;
			 gradientDirection.x = -bottomLeftIntensity - 2.0 * leftIntensity - topLeftIntensity + bottomRightIntensity + 2.0 * rightIntensity + topRightIntensity;
			 gradientDirection.y = -topLeftIntensity - 2.0 * topIntensity - topRightIntensity + bottomLeftIntensity + 2.0 * bottomIntensity + bottomRightIntensity;

			 float gradientMagnitude = length(gradientDirection);
			 vec2 normalizedDirection = normalize(gradientDirection);
			 normalizedDirection = sign(normalizedDirection) * floor(abs(normalizedDirection) + 0.617316);
			 normalizedDirection = (normalizedDirection + 1.0) * 0.5;

			 gl_FragColor = vec4(gradientMagnitude, normalizedDirection.x, normalizedDirection.y, 1.0);

		}

);

const char *raspi_gpgpu::Canny::_fragmentCanny4Src = STRINGIFY(
		 precision mediump float;
		 varying vec2 vTexCoord;
		 uniform sampler2D uInputTex;
		 uniform float iwidth;
		 uniform float iheight;
		 uniform float lowerT;
		 uniform float upperT;

		 float texelWidth = 1.0 / iwidth;
		 float texelHeight = 1.0 / iheight;
		 //float lowerThreshold = 0.035;  //  0.09;0.19 ; for color
		 //float upperThreshold = 0.11;  //  0.05;0.195/.03;.19(sensitive)   ; for b&w


		  void main()
		  {
			  vec3 currentGradientAndDirection = texture2D(uInputTex, vTexCoord).rgb;
			  vec2 gradientDirection = ((currentGradientAndDirection.gb * 2.0) - 1.0) * vec2(texelWidth, texelHeight);

			  float firstSampledGradientMagnitude = texture2D(uInputTex, vTexCoord + gradientDirection).r;
			  float secondSampledGradientMagnitude = texture2D(uInputTex, vTexCoord - gradientDirection).r;

			  float multiplier = step(firstSampledGradientMagnitude, currentGradientAndDirection.r);
			  multiplier = multiplier * step(secondSampledGradientMagnitude, currentGradientAndDirection.r);

			  float thresholdCompliance = smoothstep(lowerT, upperT, currentGradientAndDirection.r);
			  multiplier = multiplier * thresholdCompliance;

			  gl_FragColor = vec4(multiplier, multiplier, multiplier, 1.0);
		  }
);

const char *raspi_gpgpu::Canny::_fragmentCanny5Src = STRINGIFY(
		 precision mediump float;
		 varying vec2 vTexCoord;
		 uniform sampler2D uInputTex;
		 uniform float iwidth;
		 uniform float iheight;

		 float texelWidth = 1.0 / iwidth;
		 float texelHeight = 1.0 / iheight;

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
			  float bottomLeftIntensity = texture2D(uInputTex, bottomLeftTextureCoordinate).r;
				  float topRightIntensity = texture2D(uInputTex, topRightTextureCoordinate).r;
				  float topLeftIntensity = texture2D(uInputTex, topLeftTextureCoordinate).r;
				  float bottomRightIntensity = texture2D(uInputTex, bottomRightTextureCoordinate).r;
				  float leftIntensity = texture2D(uInputTex, leftTextureCoordinate).r;
				  float rightIntensity = texture2D(uInputTex, rightTextureCoordinate).r;
				  float bottomIntensity = texture2D(uInputTex, bottomTextureCoordinate).r;
				  float topIntensity = texture2D(uInputTex, topTextureCoordinate).r;
				  float centerIntensity = texture2D(uInputTex, vTexCoord).r;

				  float pixelIntensitySum = bottomLeftIntensity + topRightIntensity + topLeftIntensity + bottomRightIntensity + leftIntensity + rightIntensity + bottomIntensity + topIntensity + centerIntensity;
				  float sumTest = step(1.5, pixelIntensitySum);
				  float pixelTest = step(0.01, centerIntensity);

				  vec3 result = vec3(sumTest * pixelTest);
				  gl_FragColor = vec4(result, 1.0);
		  }
);


int raspi_gpgpu::Canny::Initialize(int inputWidth, int inputHeight,
		int outputWidth, int outputHeight, unsigned int order,
		bool externalInput) {

	printf("%s", GetProcName());
	printf( "render pass %d", _pass);

	this->CreateFBO();

	BasicProcessor::Initialize(inputWidth, inputHeight, outputWidth, outputHeight, order, externalInput);

	const char *source;
	if(_pass == 1)
		source =  _fragmentCanny1Src;
	else if(_pass == 2)
		source =  _fragmentCanny2Src;
	else if(_pass == 3)
		source =  _fragmentCanny3Src;
	else if(_pass == 4)
		source =  _fragmentCanny4Src;
	else if(_pass == 5)
		source =  _fragmentCanny5Src;

	InitFilter(source,0);

	if(_pass != 1)
		_shParamWidth = _shader->GetParameter(1, "iwidth");

	if(_pass != 2)
		_shParamHeight = _shader->GetParameter(1, "iheight");

	if(_pass == 4)
	{
		_shParamLT = _shader->GetParameter(1, "lowerT");
		_shParamUT = _shader->GetParameter(1, "upperT");
	}

	return 1;
}

void raspi_gpgpu::Canny::DoRender() {

	RenderPreinit();
	Tool::LastGLError(GetProcName(), "Canny RenderPreinit");

if(_pass != 1)
		glUniform1f(_shParamWidth, raspi_gpgpu::RaspiConfig::Instance()->Width);

	if(_pass != 2)
		glUniform1f(_shParamHeight, raspi_gpgpu::RaspiConfig::Instance()->Height);

	if(_pass == 4)
	{
			glUniform1f(_shParamLT, raspi_gpgpu::RaspiConfig::Instance()->GetLowerCannyThreshold());
			glUniform1f(_shParamUT, raspi_gpgpu::RaspiConfig::Instance()->GetUpperCannyThreshold());
	}

	


	Tool::LastGLError(GetProcName(), "Canny passing uniforms");

	RenderSetCoordinates();
	Tool::LastGLError(GetProcName(), "Canny RenderSetCoordinates");

	RenderDraw();
	Tool::LastGLError(GetProcName(), "Canny RenderDraw");

	RenderDispose();
	Tool::LastGLError(GetProcName(), "Canny RenderDispose");

}

void raspi_gpgpu::Canny::InitFrameBufferTexture() {

	assert(_frameBufferObject);
	_frameBufferObject->CreateOutputTexture(_outputFrameWidth, _outputFrameHeight);
	_outputFrameWidth = _frameBufferObject->GetTextureWidth();
	_outputFrameHeight= _frameBufferObject->GetTextureHeight();
}






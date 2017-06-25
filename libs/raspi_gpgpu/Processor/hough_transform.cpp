/*
 * hough_transform.cpp
 *
 *  Created on: 27 sty 2016
 *      Author: gregStudent
 */

#include "hough_transform.h"

/*
const char *raspi_gpgpu::HoughTransform::_fragmentHoughTransform1Src = STRINGIFY(

		 precision mediump float;
		 varying vec2 vTexCoord;
		 uniform sampler2D uInputTex;
		 float harrisConstant = 0.04;
		 float sensitivity = 0.001;

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

const char *raspi_gpgpu::HoughTransform::_fragmentHoughTransform2Src = STRINGIFY(
		 precision mediump float;

		 varying vec2 vTexCoord;
		 uniform sampler2D uInputTex;

		 // zeby nie bylo liczb ujemnych to uzywaj abs()

		 float thres = 0.05; // 0.05
		 float texelWidth   = 1.0 / 1280.0;
		 float texelHeight  = 1.0 / 768.0;
		 float texelWidth2  = 2.0 / 1280.0;
		 float texelHeight2 = 2.0 / 768.0;
		 float texelWidth3  = 3.0 / 1280.0;
		 float texelHeight3 = 3.0 / 768.0;

		 vec2 pixel1  = vTexCoord.xy + vec2(0.0, texelWidth3);
		 vec2 pixel2  = vTexCoord.xy + vec2(texelWidth, texelHeight3);
		 vec2 pixel3  = vTexCoord.xy + vec2(texelWidth2, texelHeight2);
		 vec2 pixel4  = vTexCoord.xy + vec2(texelWidth3, texelHeight);
		 vec2 pixel5  = vTexCoord.xy + vec2(texelWidth3, 0.0);
		 vec2 pixel6  = vTexCoord.xy + vec2(texelWidth3, -texelHeight);
		 vec2 pixel7  = vTexCoord.xy + vec2(texelWidth2, -texelHeight2);
		 vec2 pixel8  = vTexCoord.xy + vec2(texelWidth, -texelHeight3);
		 vec2 pixel9  = vTexCoord.xy + vec2(0.0, -texelHeight3);
		 vec2 pixel10 = vTexCoord.xy + vec2(-texelWidth, -texelHeight3);
		 vec2 pixel11 = vTexCoord.xy + vec2(-texelWidth2, -texelHeight2);
		 vec2 pixel12 = vTexCoord.xy + vec2(-texelWidth3, -texelHeight);
		 vec2 pixel13 = vTexCoord.xy + vec2(-texelWidth3, -texelHeight);
		 vec2 pixel14 = vTexCoord.xy + vec2(-texelWidth3, texelHeight);
		 vec2 pixel15 = vTexCoord.xy + vec2(-texelWidth2, texelHeight2);
		 vec2 pixel16 = vTexCoord.xy + vec2(-texelWidth, texelHeight3);



		 void main()
		 {
			   float sum = 0.0;
			   float ret = 0.0;
			   // variant 1
			   // 9.0 are much lighter that pixel P + threshold,  thres = 0.25;
			   // 0.05
//			   float centerColor = texture2D(uInputTex, vTexCoord).r + thres;
//			   sum += step(centerColor, texture2D(uInputTex, pixel1).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel2).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel3).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel4).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel5).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel6).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel7).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel8).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel9).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel10).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel11).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel12).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel13).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel14).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel15).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel16).r);

			   // variant 1 alternative

//			   float centerColor = texture2D(uInputTex, vTexCoord).r - thres;
//			   if(texture2D(uInputTex, pixel1).r > centerColor)
//				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel1).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel2).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel3).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel4).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel5).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel6).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel7).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel8).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel9).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel10).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel11).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel12).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel13).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel14).r > centerColor)
//			  				   sum += 1.0;
//			   if(texture2D(uInputTex, pixel15).r > centerColor)
//			  				   sum += 1.0;



			   // variant 2 where others are much darker that pixel P - threshold
			   float centerColor = texture2D(uInputTex, vTexCoord).r - thres;
//			   sum += step(texture2D(uInputTex, pixel1).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel2).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel3).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel4).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel5).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel6).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel7).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel8).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel9).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel10).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel11).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel12).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel13).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel14).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel15).r, centerColor);
//			   sum += step(texture2D(uInputTex, pixel16).r, centerColor);


			   // variant 2 alterantive
			   //float centerColor = texture2D(uInputTex, vTexCoord).r ;
			   //if(centerColor > 0.0)
				//   sum += 1.0;
//			   if(texture2D(uInputTex, pixel1).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel2).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel3).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel4).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel5).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel6).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel7).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel8).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel9).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel10).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel11).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel12).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel13).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel14).r < centerColor)
//							   sum += 1.0;
//			   if(texture2D(uInputTex, pixel15).r < centerColor)
//							   sum += 1.0;

			   // variant 3

			   //float centerColor = texture2D(uInputTex, vTexCoord).r - thres;
//			   sum += step(centerColor, texture2D(uInputTex, pixel14).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel15).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel3).r);
//			   sum += step(centerColor, texture2D(uInputTex, pixel4).r);


			  // variant 1/ 10.0 / 7.0 (for houghcpu) step(10.0, sum);
//			  if(centerColor.r > 0.2 && sum == 4.0)
//				  ret += 1.0;
//			  else
//				  ret = 0.0;



			  // variant 2
			 // float ret = 0.0; //step(3.0, sum);
			//  if(centerColor.r > 0.2 && sum == 2.0)
			// 		 ret = 1.0;
			//  else
			 //		 ret = 0.0;



			  if(texture2D(uInputTex, pixel14).r > centerColor  &&  texture2D(uInputTex, pixel4).r > centerColor)
				  ret += 1.0;
			  if(texture2D(uInputTex, pixel13).r > centerColor  &&  texture2D(uInputTex, pixel3).r > centerColor)
			  	 ret += 1.0;

			  if((centerColor.r > 0.2)  && ret > 0.0)
				  ret = 1.0;
			  else
				  ret = 0.0;

			  //float ret = step(4.0, sum);

		      gl_FragColor = vec4(ret, ret, ret, 1.0);
		 }
);


//const char *raspi_gpgpu::HoughTransform::_fragmentHoughTransform1Src = STRINGIFY(
//		 precision mediump float;
//		 varying vec2 vTexCoord;
//		 uniform sampler2D uInputTex;
//
//
//		 float thres = 0.05;
//
//		 void main()
//		 {
//			 for(int i = -3; i<= 3; i++)
//			 {
//			    for(int j = -3; j <=3; j++)
//			    {
//			    	float tw = float(abs(i)) / 1280.0;
//			    	float th = float(abs(j)) / 720.0;
//			    	// for this pixel in quadrat compute 15 values in hough space and write them
//			    }
//			 }
//
//			  float ret = 1.0;
//
//
//		      gl_FragColor = vec4(ret, ret, ret, 1.0);
//		 }
//);


//const char *raspi_gpgpu::HoughTransform::_fragmentHoughTransform1Src = STRINGIFY(
//		 precision mediump float;
//		 varying vec2 vTexCoord;
//		 uniform sampler2D uInputTex;
//		 const lowp float scalingFactor = 0.004;
//
//
//		 void main()
//		 {
//			  vec4 fragmentData = texture2D(uInputTex, vTexCoord);
//
//			  fragmentData.r = fragmentData.r + scalingFactor;
//			  fragmentData.g = scalingFactor * floor(fragmentData.r) + fragmentData.g;
//			  fragmentData.b = scalingFactor * floor(fragmentData.g) + fragmentData.b;
//			  fragmentData.a = scalingFactor * floor(fragmentData.b) + fragmentData.a;
//
//			  fragmentData = fract(fragmentData);
//
//			  gl_FragColor = vec4(fragmentData.rgb, 1.0);
//		 }
//);
//
//
//const char *raspi_gpgpu::HoughTransform::_fragmentHoughTransform2Src = STRINGIFY(
//		 precision mediump float;
//		 varying vec2 vTexCoord;
//		 uniform sampler2D uInputTex;
//		 float texelWidth = 1.0 / 1280.0;
//		 float texelHeight = 1.0 / 720.0;
//		 //float threshold = 0.0;
//
//		 vec2 widthStep = vec2(texelWidth, 0.0);
//		 vec2 heightStep = vec2(0.0, texelHeight);
//		 vec2 widthHeightStep = vec2(texelWidth, texelHeight);
//		 vec2 widthNegativeHeightStep = vec2(texelWidth, - texelHeight);
//
//		 vec2 leftTextureCoordinate = vTexCoord.xy - widthStep;
//		 vec2 rightTextureCoordinate = vTexCoord.xy + widthStep;
//
//		 vec2 topTextureCoordinate = vTexCoord.xy - heightStep;
//		 vec2 topLeftTextureCoordinate = vTexCoord.xy - widthHeightStep;
//		 vec2 topRightTextureCoordinate = vTexCoord.xy + widthNegativeHeightStep;
//
//		 vec2 bottomTextureCoordinate = vTexCoord.xy + heightStep;
//		 vec2 bottomLeftTextureCoordinate = vTexCoord.xy - widthNegativeHeightStep;
//		 vec2 bottomRightTextureCoordinate = vTexCoord.xy + widthHeightStep;
//
//
//		 void main()
//		 {
//			  float bottomColor = texture2D(uInputTex, bottomTextureCoordinate).r;
//			  float bottomLeftColor = texture2D(uInputTex, bottomLeftTextureCoordinate).r;
//			  float bottomRightColor = texture2D(uInputTex, bottomRightTextureCoordinate).r;
//			  vec4 centerColor = texture2D(uInputTex, vTexCoord);
//			  float leftColor = texture2D(uInputTex, leftTextureCoordinate).r;
//			  float rightColor = texture2D(uInputTex, rightTextureCoordinate).r;
//			  float topColor = texture2D(uInputTex, topTextureCoordinate).r;
//			  float topRightColor = texture2D(uInputTex, topRightTextureCoordinate).r;
//			  float topLeftColor = texture2D(uInputTex, topLeftTextureCoordinate).r;
//
//			  // Use a tiebreaker for pixels to the left and immediately above this one
//			  float multiplier = 1.0 - step(centerColor.r, topColor);
//			  multiplier = multiplier * (1.0 - step(centerColor.r, topLeftColor));
//			  multiplier = multiplier * (1.0 - step(centerColor.r, leftColor));
//			  multiplier = multiplier * (1.0 - step(centerColor.r, bottomLeftColor));
//
//			  float maxValue = max(centerColor.r, bottomColor);
//			  maxValue = max(maxValue, bottomRightColor);
//			  maxValue = max(maxValue, rightColor);
//			  maxValue = max(maxValue, topRightColor);
//
//			  float finalValue = centerColor.r * step(maxValue, centerColor.r) * multiplier;
//			  //finalValue = step(threshold, finalValue);
//
//			  gl_FragColor = vec4(finalValue, finalValue, finalValue, 1.0);
//		 }
//);

*/

const char *raspi_gpgpu::HoughTransform::_fragmentHoughTransform1Src = STRINGIFY(
		precision mediump float;
		//varying vec2 vTexCoord;
		//varying vec2 orgTexCoord;
		//uniform sampler2D uInputTex;
		varying float vFragAddVal;
		void main() {
			//gl_FragColor = texture2D(uInputTex, gl_PointCoord); //gl_PointCoord
			gl_FragColor = vec4(vec3(vFragAddVal), 1.0);
			//gl_FragColor = texture2D(uInputTex, vTexCoord);
			//gl_FragColor = vec4(1.0);
//			float color = texture2D(uInputTex,  vTexCoord).r;
//			if(color > 0.0)
//				gl_FragColor = vec4(vec3(0.9), 1.0);

			}
);

const char *raspi_gpgpu::HoughTransform::_fragmentHoughTransform2Src = STRINGIFY(
);




int raspi_gpgpu::HoughTransform::Initialize(int inputWidth, int inputHeight,
		int outputWidth, int outputHeight, unsigned int order,
		bool externalInput) {

	printf("%s", GetProcName());
	printf( "render pass %d", _pass);

	this->CreateFBO();

	BasicProcessor::Initialize(inputWidth, inputHeight, outputWidth, outputHeight, order, externalInput);

	const char *source;
	assert(_pass > 0);
	assert(_pass < 30);



	if(_pass == 1)
		source =  _fragmentHoughTransform1Src;



	InitFilter(source,0);

	//if(_pass == 1 )
	//	_shHoughSpace = _shader->GetParameter(1, "hough");

	return 1;
}

void raspi_gpgpu::HoughTransform::DoRender() {

	RenderPreinit();
	Tool::LastGLError(GetProcName(), "Hough RenderPreinit");


//	if(_pass == 1)
//	{
//		float t[] = {0.0, 1.0};
//		glUniform2fv(_shParamStep, 1, t);
//	}
//	else if(_pass == 2)
//	{
//		float t[] = {1.0, 0.0};
//		glUniform2fv(_shParamStep, 1, t);
//	}


	Tool::LastGLError(GetProcName(), "Hough passing uniforms");

	// special case for first pass
	// execute draw for 30 angles
	if(_pass == 1)
	{      
      raspi_gpgpu::RaspiConfig *cnf = raspi_gpgpu::RaspiConfig::Instance();
      if(cnf->_angle == 200)
      {
		for(int i = 0; i < 30; i++)
		{
			if(i == 0)
			{
				RenderSetCoordinates();
				Tool::LastGLError(GetProcName(), "Hough RenderSetCoordinates");
			}
			SetHoughAttributes();
			RenderDraw();
			Tool::LastGLError(GetProcName(), "Hough RenderDraw");

			this->_angleParam += 6.0;
		}
      }
      else
      {
         std::cout<<"Angle: "<< cnf->_angle << std::endl; 
         this->_angleParam = cnf->_angle;
         RenderSetCoordinates();
			Tool::LastGLError(GetProcName(), "Hough RenderSetCoordinates");
			
			SetHoughAttributes();
			RenderDraw();
			Tool::LastGLError(GetProcName(), "Hough RenderDraw");
   
      }
	}
	else
	{
		RenderSetCoordinates();
		Tool::LastGLError(GetProcName(), "Hough RenderSetCoordinates");

		RenderDraw();
		Tool::LastGLError(GetProcName(), "Hough RenderDraw");
	}


	RenderDispose();
	Tool::LastGLError(GetProcName(), "Hough RenderDispose");

}

void raspi_gpgpu::HoughTransform::InitFrameBufferTexture() {

	assert(_frameBufferObject);
	_frameBufferObject->CreateOutputTexture(_outputFrameWidth, _outputFrameHeight);
	_outputFrameWidth = _frameBufferObject->GetTextureWidth();
	_outputFrameHeight= _frameBufferObject->GetTextureHeight();
}

void raspi_gpgpu::HoughTransform::SetHoughAttributes() {
	glVertexAttrib1f(_aVertexAngleParam, this->_angleParam);
	Tool::LastGLError("HoughTransform::SetHoughAttributes", "SetHoughAttributes");
}

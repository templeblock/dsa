/*
 * max_filter.cpp
 *
 *  Created on: 18 mar 2016
 *      Author: gregStudent
 */


#include "max_filter.h"


const char *raspi_gpgpu::MaxProcessor::_fragmentShaderSource =  STRINGIFY(
		 precision highp float;
		 varying vec2 vTexCoord;
		 uniform sampler2D uInputTex;

		 float texelWidth   = 1.0 / 640.0;
		 float texelHeight  = 1.0 / 480.0;



		 void main() {
			 vec4  centerPixel = texture2D(uInputTex, vTexCoord);
			 float centerIntensity = centerPixel.r;

			 if(centerIntensity > 0.11 && centerIntensity < 0.4)
			 {
				int range = 4;
      

				for(int i = -4; i <= 4; i+=2)
				{
               
					for(int j = -2; j<= 2; j+=2)
					{
                  if(i != 0 && j != 0)
                  {  
						vec2 loc = vTexCoord + vec2(float(i) * texelWidth, float(j) * texelHeight);
						vec4 pixel = texture2D(uInputTex, loc);

						if(pixel.r < 0.4 && pixel.r >= centerIntensity)
						{
							centerPixel = centerPixel * 0.0;
							
						}
                  }
					}
               
				}

				gl_FragColor = centerPixel;
			 }
			 else
				 gl_FragColor = vec4(0.0);

		 }
	);

/*
const char *raspi_gpgpu::MaxProcessor::_fragmentShaderSource =  STRINGIFY(
		 precision lowp float;
		 varying vec2 vTexCoord;
		 uniform sampler2D uInputTex;

		 void main() {
			 vec4  centerPixel = texture2D(uInputTex, vTexCoord);
			 float centerIntensity = centerPixel.r;

			 if(centerIntensity > 0.11 && centerIntensity < 0.4)
			 {
				 gl_FragColor = centerPixel;
			 }
			 else
				 gl_FragColor = vec4(0.0);

		 }
	);
*/
/*
const char *raspi_gpgpu::MaxProcessor::_fragmentShaderSource =  STRINGIFY(
		 precision highp float;
		 varying vec2 vTexCoord;
		 uniform sampler2D uInputTex;

		 float texelWidth   = 1.0 / 1280.0;
		 float texelHeight  = 1.0 / 768.0;

		 void main() {
			 vec4  centerPixel = texture2D(uInputTex, vTexCoord);
			 float centerIntensity = centerPixel.r;

			 if(centerIntensity > 0.08 && centerIntensity < 0.4)
			 {
			    float w = 1280.0;
				float h = 768.0;
				float x = vTexCoord.x * w ;
			    float y = vTexCoord.y * h;
				int range = 4;
			    //float hough_h = ((sqrt(2.0) * w) / 2.0);
				//int aH = int(hough_h * 2.0);
				float aW = 180.0;
				int index = int((y * w)) + int(x);  //ceil
				int r = int(floor(float(index) / aW)); //ceil
				int t = index - (r * int(aW));
				int found = 0;

				for(int j = -range; j <= range; j++)
				{
					for(int i = -range; i <= range; i++)
					{
						if(j == 0 && i == 0)
							continue;

						int locIdx = int(((r + j) * int(aW))) + (t + i); //ceil
						// map locIdx back to original image size
						int newy = int(floor(float(locIdx) / w)); //ceil
						int newx = locIdx - (newy * int(w));
						vec2 loc = vec2(float(newx) / w, (float(newy) / h));
						//vec2 loc = vTexCoord + vec2((x + float(newx)) * texelWidth, (y + float(newy)) * texelHeight);
						float pixel = texture2D(uInputTex, loc).r;

						if(pixel > centerIntensity)
						{
							//centerPixel = centerPixel * 0.0;
							centerPixel = vec4(1.0, 0.0, 1.0, 1.0);
							found = 1;
							i = range;
							j = range;
						}
					}
				}
				if(found == 1)
					gl_FragColor = centerPixel;
				else
					gl_FragColor = centerPixel * 0.0;;
			 }
			 else
				 gl_FragColor = vec4(0.0);

		 }
	);
*/
raspi_gpgpu::MaxProcessor::MaxProcessor() {
}

int raspi_gpgpu::MaxProcessor::Initialize(int inputWidth, int inputHeight,
	int outputWidth, int outputHeight, unsigned int order,
	bool externalInput) {

	std::cout<< GetProcName() << " Raspi_gpgpu::MaxProcessor::Initialize" << std::endl;
	this->CreateFBO();
	BasicProcessor::Initialize(inputWidth, inputHeight, outputWidth, outputHeight, order, externalInput);
	InitFilter(_fragmentShaderSource);
	return (1);
}

void raspi_gpgpu::MaxProcessor::DoRender() {
	printf("%s INtex %d, target %d, fbo %dx%d", GetProcName(), _inTextureId, _inTextureTarget, _outputFrameWidth, _outputFrameHeight);

	RenderPreinit();
	Tool::LastGLError(GetProcName(), "MaxProcessor RenderPreinit");

	RenderSetCoordinates();
	Tool::LastGLError(GetProcName(), "MaxProcessor RenderSetCoordinates");

	RenderDraw();
	Tool::LastGLError(GetProcName(), "MaxProcessor RenderDraw");

	RenderDispose();
	Tool::LastGLError(GetProcName(), "MaxProcessor RenderDispose");
}

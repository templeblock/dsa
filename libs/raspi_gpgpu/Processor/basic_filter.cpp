/*
 * basic_filter.cpp
 *
 *  Created on: 11 sty 2016
 *      Author: gregStudent
 */


#include "basic_filter.h"
#include <string>
#include "hough_transform.h"

const  char *raspi_gpgpu::BasicFilter::_vertexShaderDefault = STRINGIFY (
				  attribute vec4 aPos;
				  attribute vec2 aTexCoord;
				  varying vec2 vTexCoord;
				  void main() {
					  vTexCoord = aTexCoord;
					  //gl_PointSize = 2.0;
					  gl_Position = aPos;

					  //vec3 pixel = texture2D(uInputTex, vTexCoord).rgb
					  //vTexCoord = aTexCoord; // - vec2(texelWidth * float(30),0);

//					  1. normalize vertex to range [0..1]
//					   vec2 coords = aPos.st * vec2(0.5) + vec2(0.5);
//
//					  2. normaliye range [0..1] back to vertex original range
//
//					   vec2 back = (2.0 * coords) - 1.0;

				  }
			  );

/*
const  char *raspi_gpgpu::BasicFilter::_vertexShaderHough = STRINGIFY (
		 attribute vec4 aPos;
		  attribute vec2 aTexCoord;
		  varying vec2 vTexCoord;
		  attribute float aAngle;
		  uniform sampler2D uInputTex;
		  void main() {
			  vTexCoord = aTexCoord;
			  gl_PointSize = 2.0;
			  vec4 pixel = texture2D(uInputTex, vTexCoord);
			  if(texture2D(uInputTex, vTexCoord).a > 0.1)
				  gl_Position = aPos;
			  else
				  gl_Position = vec4(aPos.x * (-1.0), aPos.y * (-1.0), aPos.z, aPos.w);
		  }

	);


	//		  if(aAngle == 120.0)   // wtf ? newx == 340
//		  {
//			  vec2 back = (2.0 * coords) - 1.0;
//			  gl_Position = vec4(back.x , back.y * (-1.0), 0.0, 1.0);
//
//		  }
//		  else
//			  gl_Position = aPos;

		  //gl_Position = vec4(coords.x * w  , coords.y * h, 0.0, 1.0);
		  //gl_Position = vec4(coords.x - 1.0  , coords.y, 0.0, 1.0);
		  //gl_Position = vec4(vTexCoord, 0.0, 1.0);
		 //vFragAddVal = texture2D(uInputTex, aTexCoord).r;
		 //gl_Position = aPos;
*/


const  char *raspi_gpgpu::BasicFilter::_vertexShaderHough = STRINGIFY (
	  precision mediump float;
     attribute vec2 aTexCoord;
	  attribute vec4 aPos;	  
	  attribute float aAngle;	 
	  varying float vFragAddVal;
	  uniform sampler2D uInputTex;
	  float DEG2RAD = 0.017453293;
	  float w = 640.0;
	  float h = 480.0;


	  

	  void main() {
        vec3 pexel = texture2D(uInputTex, aTexCoord).rgb;
		  if(pexel.x > 0.85)
		  {
			  vec2 coords = aPos.st * vec2(0.5) + vec2(0.5);

			  float center_x = w/2.0;
			  float center_y = h/2.0;
			  float hough_h = ((sqrt(2.0) * w) / 2.0);
			  float x = coords.x * w ;
			  float y = coords.y * h;
			  float r = ( (x - center_x) * cos(aAngle * DEG2RAD)) + ((y - center_y) * sin(aAngle * DEG2RAD));
          
			  float s = floor(r + hough_h);
			  int index = int(s * 180.0) + int(aAngle);
			  int newy = int(float(index) / w);
			  int newx = index - (newy * int(w));
			  coords = vec2(float(newx) / w, float(newy) / h); 

			  vec2 back = (2.0 * coords) - 1.0;
			  gl_Position = vec4(back.x, back.y, 0.0, 1.0);			  
			  vFragAddVal = 0.004;
		  }
		  else
		  {			  
			  gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
		  }
		 
		 gl_PointSize = 1.0;       
	  }
);



/*
const  char *raspi_gpgpu::BasicFilter::_vertexShaderHough = STRINGIFY (
		  //attribute vec4 aPos;
		  attribute vec2 aTexCoord;
		  //varying vec2 vTexCoord;
		  //varying vec2 orgTexCoord;
		  varying float vFragAddVal;
		  uniform sampler2D uInputTex;
		  attribute float aTSCoordType;

		  const mat4 tsCoordFactors = mat4(
			   0, -1, 0, 0,  // t begin
			   0,  1, 0, 0,  // t end
			   0,  1, 0, 0,  // s begin
			   0,  1, 0, 0   // s end
		  );
		  const mat4 tsCoordAdd = mat4(
			  -1,  0, 0, 0,  // t begin
			   0,  0, 0, 0,  // t end
			   0,  0, 0, 0,  // s begin
			   1,  0, 0, 0   // s end
		  );

		void main() {

			// get texture sample (binary value)
			float bin = texture2DLod(uInputTex, aTexCoord, 0.0).r;
			// get the provided index that selects a coordinate factor and summand
			// depending on: start or end point? T or S space?
			int idx = int(aTSCoordType);
			vec2 coordFact = tsCoordFactors[idx].xy;
			vec2 coordAdd = tsCoordAdd[idx].xy;
			// normalize the coordinates and create a flipped version
			vec2 normCoordStd = -1.0 * aTexCoord + 2.0 * aTexCoord;
			vec2 normCoordFlipped = normCoordStd.yx;
			vec2 finalCoord = vec2(0, 0);
			// select the standard or the flipped version
			// depending on the provided TS coord select index
			if (idx == 0 || idx == 3) {
				finalCoord = normCoordStd;
			} else {
				finalCoord = normCoordFlipped;
			}
			// calculate one line point in ST space
			vec2 linePoint = bin * (coordFact * finalCoord + coordAdd);
			// set the value that will be added in the fragment shader
			vFragAddVal = bin * (200.0 / 256.0);
			// set the position of the line point
			gl_Position = vec4(linePoint, 0.0, 1.0);
			//gl_Position = aPos;
		}
);
*/

raspi_gpgpu::BasicFilter::~BasicFilter() {
	glDeleteBuffers(2, this->vBuff);
   if(_TSCoordsBufferFull && IsHoughProc())
		delete _TSCoordsBufferFull;
	if(_textureCoordsBufferFull && IsHoughProc())
		delete _textureCoordsBufferFull;
}


void raspi_gpgpu::BasicFilter::SetOutputRenderOrientation(int orientation) {
	   this->InitTextureCoordianates() ;
}


void raspi_gpgpu::BasicFilter::UseTexture(GLuint id, GLuint useTextureUnit,
		GLenum target) {
	_inTextureId = id;
	_intextureUnit = useTextureUnit;

	if (target != _inTextureTarget) {
		if (_fragmentShaderSourceCompilation) {
			DoFilterSetup(_fragmentShaderSourceCompilation, target);
		}

		_inTextureTarget = target;
	}
}


void raspi_gpgpu::BasicFilter::InitFilter(const char* filterShaderSource, int o) {
	DoFilterSetup(filterShaderSource, _inTextureTarget);
	memcpy(_vertexBuffer, BasicProcessor::Vertices, gpgpu_vertexBufferSize * sizeof(GLfloat));
	InitTextureCoordianates(o);
}


void raspi_gpgpu::BasicFilter::DoFilterSetup(const char* filterShaderSource,
		GLenum target) {
   // check inherited class
	if(IsHoughProc())
	{
		BasicProcessor::InitShader(_vertexShaderHough, filterShaderSource, target);

		if(!_TSCoordsBufferFull || !_textureCoordsBufferFull)
		{
			if(_TSCoordsBufferFull)
				delete _TSCoordsBufferFull;
			if(_textureCoordsBufferFull)
				delete _textureCoordsBufferFull;

			//int numTexCoords = 1280 * 768 * 8 / 2;
			int numTexCoords = 640 * 480 * 3;
			//int numTexCoordsPairs = numTexCoords / 2;
			int numTexCoordsPairs = numTexCoords / 3;
			_textureCoordsBufferFull = new GLfloat[numTexCoords];
			_TSCoordsBufferFull = new GLfloat[numTexCoordsPairs * 2];


			int idx =  0;
			int idx2 = 0;


//			for (int y = 0; y < 768; y+=2) {
//						for (int x = 0; x < 1280; x+=2) {
//							const float texX = (float)x / 1280.0;
//							const float texY = (float)y / 768.0;
//
//							for (int i = 0; i < 8; i+=2) {
//								_textureCoordsBufferFull[idx] =    texX;
//								_textureCoordsBufferFull[idx+1] =  texY;
//
//								_TSCoordsBufferFull[idx2] = (GLfloat)(floor(i / 2));
//
//								idx+=2;
//								idx2++;
//							}
//						}
//					}

			for (int y = -480; y < 480; y+=2) {
			for (int x = -640; x < 640; x+=2) {

					const float texX = (float)x / 640.0;
					const float texY = (float)y / 480.0;
					_textureCoordsBufferFull[idx] =    texX;
					_textureCoordsBufferFull[idx+1] =  texY;
					_textureCoordsBufferFull[idx+2] =  0.0;
					idx+=3;

				}
			}

			for (int y = 0; y < 480; y++) {
			for (int x = 0; x < 640; x++) {

					const float texX = (float)x / 640.0;
					const float texY = (float)y / 480.0;
					_TSCoordsBufferFull[idx2] =    texX;
					_TSCoordsBufferFull[idx2+1] =  texY;
					idx2+=2;
				}
			}

			// write vertex texture coords and texture coords into VBO
			//glEnableClientState(GL_VERTEX_ARRAY);
			glGenBuffers(2, this->vBuff);

			glBindBuffer(GL_ARRAY_BUFFER, vBuff[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numTexCoords, _textureCoordsBufferFull, GL_STATIC_DRAW); //GL_STATIC_DRAW
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			Tool::LastGLError("raspi_gpgpu::BasicFilter::DoFilterSetup", "Set VBO 1");

			glBindBuffer(GL_ARRAY_BUFFER, vBuff[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numTexCoordsPairs * 2, _TSCoordsBufferFull, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			Tool::LastGLError("raspi_gpgpu::BasicFilter::DoFilterSetup", "Set VBO 2");
		}
	}
	else
		BasicProcessor::InitShader(_vertexShaderDefault, filterShaderSource, target);


//	if(IsHoughProc())
//		_aShaderParamATSCoordType = _shader->GetParameter(0, "aTSCoordType");
//	else
	_pShaderPos = _shader->GetParameter(0, "aPos");
	_aShaderTextureCoordinates = _shader->GetParameter(0, "aTexCoord");

	if(IsHoughProc())
		_aVertexAngleParam = _shader->GetParameter(0, "aAngle");

	_shaderParamUInputTex = _shader->GetParameter(1, "uInputTex");
	_fragmentShaderSourceCompilation = filterShaderSource;
}


void raspi_gpgpu::BasicFilter::InitTextureCoordianates(int setting) {
	const GLfloat  *p = BasicProcessor::TexCoordArray;
	const GLfloat  *p2 = BasicProcessor::TexCoordDiagonalArray;
	if(setting == 1)
		memcpy(_textureCoordsBuffer, p2, gpgpu_textureBufferSize * sizeof(GLfloat));
	else
		memcpy(_textureCoordsBuffer, p, gpgpu_textureBufferSize * sizeof(GLfloat));
}


void raspi_gpgpu::BasicFilter::RenderPreinit() {
	_shader->Execute();
	glViewport(0, 0, _outputFrameWidth, _outputFrameHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0 + _intextureUnit);
	glBindTexture(_inTextureTarget, _inTextureId);
	glUniform1i(_shaderParamUInputTex, _intextureUnit);
}


void raspi_gpgpu::BasicFilter::RenderSetCoordinates() {
	if (_frameBufferObject)
		_frameBufferObject->Bind();


	if(IsHoughProc())
	{
		glBindBuffer(GL_ARRAY_BUFFER, vBuff[0]);
		Tool::LastGLError("raspi_gpgpu::BasicFilter::RenderSetCoordinates", "Bind Buffer 1");
	}


	glEnableVertexAttribArray(_pShaderPos);
	Tool::LastGLError("raspi_gpgpu::BasicFilter::RenderSetCoordinates", "RenderSetCoordinates 1");

	if(IsHoughProc())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		// enable blending
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);
//
//		glEnableVertexAttribArray(_aShaderParamATSCoordType);
//		Tool::LastGLError("raspi_gpgpu::BasicFilter::RenderSetCoordinates", "RenderSetCoordinates 1");
//		glVertexAttribPointer(_aShaderParamATSCoordType,
//							  1,
//							  GL_FLOAT,
//							  GL_FALSE,
//							  0,
//							  _TSCoordsBufferFull);


//		glVertexAttribPointer(_pShaderPos,
//								  3,
//								  GL_FLOAT,
//								  GL_FALSE,
//								  0,
//								  _textureCoordsBufferFull);


		glVertexAttribPointer(_pShaderPos,
							  3,
							  GL_FLOAT,
							  GL_FALSE,
							  0,
							  0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
	{

		glVertexAttribPointer(_pShaderPos,
								  3,
								  GL_FLOAT,
								  GL_FALSE,
								  0,
								  _vertexBuffer);
	}

	Tool::LastGLError("raspi_gpgpu::BasicFilter::RenderSetCoordinates", "RenderSetCoordinates 2");


	if(IsHoughProc())
	{
		glBindBuffer(GL_ARRAY_BUFFER,vBuff[1]);
		Tool::LastGLError("raspi_gpgpu::BasicFilter::RenderSetCoordinates", "Bind Buffer 2");
	}

	glEnableVertexAttribArray(_aShaderTextureCoordinates);
	Tool::LastGLError("raspi_gpgpu::BasicFilter::RenderSetCoordinates", "RenderSetCoordinates 3");

	if(IsHoughProc())
	{
//		glVertexAttribPointer(_aShaderTextureCoordinates,
//									  2,
//									  GL_FLOAT,
//									  GL_FALSE,
//									  0,
//									  _textureCoordsBufferFull);

//		glVertexAttribPointer(_aShaderTextureCoordinates,
//									  2,
//									  GL_FLOAT,
//									  GL_TRUE,
//									  0,
//									  _TSCoordsBufferFull);

		glVertexAttribPointer(_aShaderTextureCoordinates,
							  2,
							  GL_FLOAT,
							  GL_TRUE,
							  0,
							  0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	else
	{
		glVertexAttribPointer(_aShaderTextureCoordinates,
							  2,
							  GL_FLOAT,
							  GL_FALSE,
							  0,
							  _textureCoordsBuffer);
	}
	Tool::LastGLError("raspi_gpgpu::BasicFilter::RenderSetCoordinates", "RenderSetCoordinates 4");



	// send angle attribute
	// being set in HoughTransform::SetHoughAttributes
	//glVertexAttrib1f(_aVertexAngleParam, this->_angleParam);

}


void raspi_gpgpu::BasicFilter::RenderDraw() {

	if(IsHoughProc())
	{
		//std::cout<< std::endl << "drawing in hough for angle " << this->_angleParam << std::endl;
		//glDrawArrays(GL_LINES, 0, 1966080);
		glDrawArrays(GL_POINTS, 0, 307200); // 983040
		//glDrawElements(GL_POINTS, 983040, GL_FLOAT, 0);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	else
	{		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}


void raspi_gpgpu::BasicFilter::RenderDispose() {
	if(IsHoughProc())
	{
		glDisable(GL_BLEND);
		//glDisableVertexAttribArray(_aShaderParamATSCoordType);
		glDisableVertexAttribArray(_pShaderPos);
	}
	else
		glDisableVertexAttribArray(_pShaderPos);


	glDisableVertexAttribArray(_aShaderTextureCoordinates);

	if(IsHoughProc())
	{
		glDisableVertexAttribArray(_aVertexAngleParam);
	}

	if (_frameBufferObject)
		_frameBufferObject->Unbind();
}

int raspi_gpgpu::BasicFilter::IsHoughProc() {
	  if(_isHoughProc == -1)
	  {
		  raspi_gpgpu::HoughTransform* p = dynamic_cast<raspi_gpgpu::HoughTransform*>(this);
		  if(p)
			  _isHoughProc = 1;
		  else
			  _isHoughProc = 0;
	  }

	  return _isHoughProc;
}

void raspi_gpgpu::BasicFilter::ResetProcVariables() {
	 this->_angleParam = 0.0;
}

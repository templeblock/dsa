/*
 * ShaderFactory.cpp
 *
 *  Created on: 11 sty 2016
 *      Author: gregStudent
 */

#include "ShaderFactory.h"


raspi_gpgpu::ShaderFactory::ShaderFactory() {
	programId = 0;
}

raspi_gpgpu::ShaderFactory::~ShaderFactory() {
	if (programId > 0)
				glDeleteProgram(programId);
}

bool raspi_gpgpu::ShaderFactory::Build(const char* fragmentSource, const char* vertexSource) {
	programId = Create(vertexSource, fragmentSource, &vertexId, &fragmentId);
	return (programId > 0);
}

void raspi_gpgpu::ShaderFactory::Execute() {
	glUseProgram(programId);
}

GLint raspi_gpgpu::ShaderFactory::GetParameter(int type, const char* name) const {
	GLint id = (type == 0) ?
	        glGetAttribLocation(programId, name) :
	        glGetUniformLocation(programId, name);

	if (id < 0) {
		std::cout << "ShaderFactory::GetParameter: error for param " << name << std::endl;
		Tool::LastGLError("raspi_gpgpu::ShaderFactory::GetParameter", "param not found");
	}


	return id;
}

GLuint raspi_gpgpu::ShaderFactory::Create(const char* vertexSource,
		const char* fragmentSource, GLuint* vertexId, GLuint* fragmentId) {

	*vertexId = Compile(GL_VERTEX_SHADER, vertexSource);
	*fragmentId = Compile(GL_FRAGMENT_SHADER, fragmentSource);

	GLuint programId = glCreateProgram();

	if (programId == 0) {
		std::cout <<"aspi_gpgpu::ShaderFactory::Create error creating program";
		return 0;
	}

	glAttachShader(programId, *vertexId);
	glAttachShader(programId, *fragmentId);
	glLinkProgram(programId);


	GLint linkStatus;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		std::cout <<" raspi_gpgpu::ShaderFactory::Create error in linking program";
		GLchar log[1024];
		GLsizei len;
		glGetProgramInfoLog(programId, 1024, &len, log);
		std::cout << log << std::endl;
		glDeleteProgram(programId);
		return 0;
	}

	return programId;
}

GLuint raspi_gpgpu::ShaderFactory::Compile(GLenum type, const char* source) {
	GLuint shaderId = glCreateShader(type);

	if (shaderId == 0) {
		std::cout<<("ShaderFactory::Compile, error in creating shader");
		return 0;
	}

	glShaderSource(shaderId, 1, (const GLchar**)&source, NULL);
	glCompileShader(shaderId);
	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE) {
		std::cout<<("Shader", "ShaderFactory::Compile , error compiling");
		GLchar infoLogBuf[1024];
		GLsizei infoLogLen;
		glGetShaderInfoLog(shaderId, 1024, &infoLogLen, infoLogBuf);
		std::cout << infoLogBuf << std::endl << std::endl;
		std::cout << source << std::endl << std::endl;
		glDeleteShader(shaderId);
		return 0;
	}

	return shaderId;
}






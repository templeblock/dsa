/*
 * ShaderFactory.h
 *
 *  Created on: 11 sty 2016
 *      Author: gregStudent
 */

#ifndef SHADERFACTORY_H_
#define SHADERFACTORY_H_

#include "../egl_include.h"

namespace raspi_gpgpu
{
	class ShaderFactory {
	public:

		ShaderFactory();

		~ShaderFactory();

		bool Build(const char *fragmentSource, const char *vertexSource);

		void Execute();

		GLint GetParameter(int type, const char *name) const;

	private:

		static GLuint Create(const char *vertexSource, const char *fragmentSource, GLuint *vertexId, GLuint *fragmentId);

		static GLuint Compile(GLenum type, const char *source);

		GLuint vertexId;

		GLuint fragmentId;

		GLuint programId;

	};

}

#endif /* SHADERFACTORY_H_ */

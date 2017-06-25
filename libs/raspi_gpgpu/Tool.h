/*
 * Tool.h
 *
 *  Created on: 15 gru 2015
 *      Author: gregStudent
 */

#ifndef TOOL_H_
#define TOOL_H_

#include "egl_include.h"
#include <string>
#include <cstdio>

namespace raspi_gpgpu
{
	class Tool
	{
		public:
		static void LastGLError(const char *str, const char *message)
		{
			GLenum error = glGetError();
				if (error) {
			        std::cout<< std::endl << str << " - GLES error " << error << " occured " << message << std::endl;
				}
		}
	};
}


#endif /* TOOL_H_ */

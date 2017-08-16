#ifndef IAITO_OPENGL_HPP
#define IAITO_OPENGL_HPP

#define GLEW_STATIC
#include "GL/gl3w.h"

#include <GLFW/glfw3.h>

#include <string>

namespace iaito
{
	#ifdef IAITO_DEBUG
		#define glCheck(call) call; glCheckError(__FILE__, __LINE__);
	#else
		#define glCheck(call) (call)
	#endif

	void glCheckError(const std::string& file, unsigned int line);
}

#endif // IAITO_OPENGL_HPP
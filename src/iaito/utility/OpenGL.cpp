#include "iaito/utility/OpenGL.hpp"

#include "iaito/utility/Log.hpp"

namespace iaito
{
	void glCheckError(const std::string& file, unsigned int line)
	{
		GLenum code = glGetError();

		if (code != GL_NO_ERROR)
		{
			std::string(file);
			std::string error = "unknown error";

			switch (code)
			{
				case GL_INVALID_ENUM:
					error = "GL_INVALID_ENUM";
					break;

				case GL_INVALID_VALUE:
					error = "GL_INVALID_VALUE";
					break;

				case GL_INVALID_OPERATION:
					error = "GL_INVALID_OPERATION";
					break;

				case GL_INVALID_FRAMEBUFFER_OPERATION:
					error = "GL_INVALID_FRAMEBUFFER_OPERATION";
					break;

				case GL_OUT_OF_MEMORY:
					error = "GL_OUT_OF_MEMORY";
					break;

				case GL_STACK_UNDERFLOW:
					error = "GL_STACK_UNDERFLOW";
					break;

				case GL_STACK_OVERFLOW:
					error = "GL_STACK_OVERFLOW";
					break;
			}

			Log(Error) 	<< "internal OpenGL error in " << file << " : " << line << " - " << error;
		}
	}
}
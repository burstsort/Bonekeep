#ifndef IAITO_SHADER_HPP
#define IAITO_SHADER_HPP

#include <string>

#include "iaito/core/Resource.hpp"
#include "iaito/utility/OpenGL.hpp"

namespace iaito
{
	class Shader: public Resource
	{
		public:
			Shader();

			void Load(const std::string& filename);
			void Unload();

			void CreateProgram();
			void DestroyProgram();

			GLuint GetProgram();

		private:
			std::string _filename;
			
			GLuint _program;
	};
}

#endif // IAITO_SHADER_HPP
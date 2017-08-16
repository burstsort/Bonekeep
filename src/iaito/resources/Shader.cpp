#include "iaito/resources/Shader.hpp"

#include <fstream>
#include "iaito/utility/Log.hpp"

namespace iaito
{
	Shader::Shader() : _program(0){}

	void Shader::Load(const std::string& filename)
	{
		Unload();

		// Store filename
		_filename = filename;

		Log(Debug) << "Loading shader: " << _filename;

		CreateProgram();
	}

	void Shader::Unload()
	{
		if (_filename.empty())
			return;
		
		Log(Debug) << "Unloading shader: " << _filename;

		DestroyProgram();

		_filename.clear();
	}

	void Shader::CreateProgram()
	{
		if (_filename.empty() || _program)
			return;

		std::string vs, fs;

		std::ifstream vf((_filename+".vsh").c_str());
		std::ifstream ff((_filename+".fsh").c_str());

		if (vf.good())
		{
			// Load vertex shader
			vf.seekg(0, std::ios::end);
			vs.reserve(vf.tellg());
			vf.seekg(0, std::ios::beg);
			vs.assign((std::istreambuf_iterator<char>(vf)), std::istreambuf_iterator<char>());
		}
		else
		{
			Log(Error) << "Error reading vertex shader source from " << _filename << ".vsh";
			return;
		}

		if (ff.good())
		{
			// Load fragment shader
			ff.seekg(0, std::ios::end);
			fs.reserve(ff.tellg());
			ff.seekg(0, std::ios::beg);
			fs.assign((std::istreambuf_iterator<char>(ff)), std::istreambuf_iterator<char>());
		}
		else
		{
			Log(Error) << "Error reading fragment shader source from " << _filename << ".fsh";
			return;
		}

		const char* vc = vs.c_str();
		const char* fc = fs.c_str();
		GLint status;

		// Create and compile the vertex shader
		GLuint vi = glCreateShader(GL_VERTEX_SHADER);
		if (!vi)
		{
			Log(Error) << "Error creating a vertex shader";
			return;
		}

		glCheck(glShaderSource(vi, 1, &vc, NULL));
		glCheck(glCompileShader(vi));

		// Check vertex shader compile status
		glCheck(glGetShaderiv(vi, GL_COMPILE_STATUS, &status));
		if (!status)
		{
			char log[512];
			glCheck(glGetShaderInfoLog(vi, 512, NULL, log));
			Log(Error) << "Vertex shader compilation failed:\n" << log;
		}

		// Create and compile the fragment shader
		GLuint fi = glCreateShader(GL_FRAGMENT_SHADER);
		if (!fi)
		{
			glCheck(glDeleteShader(vi));
			Log(Error) << "Error creating a fragment shader";
			return;
		}

		glCheck(glShaderSource(fi, 1, &fc, NULL));
		glCheck(glCompileShader(fi));

		// Check fragment shader compile status
		glCheck(glGetShaderiv(fi, GL_COMPILE_STATUS, &status));
		if (!status)
		{
			char log[512];
			glCheck(glGetShaderInfoLog(fi, 512, NULL, log));
			Log(Error) << "Fragment shader compilation failed:\n" << log;
		}

		// Create and link the shader program
		_program = glCreateProgram();
		if (!_program)
		{
			glCheck(glDeleteShader(vi));
			glCheck(glDeleteShader(fi));
			Log(Error) << "Error creating a shader program";
			return;
		}

		glCheck(glAttachShader(_program, vi));
		glCheck(glAttachShader(_program, fi));

		glCheck(glLinkProgram(_program));

		glCheck(glDetachShader(_program, vi));
		glCheck(glDetachShader(_program, fi));

		glCheck(glDeleteShader(vi));
		glCheck(glDeleteShader(fi));

		// Check shader program link status
		glCheck(glGetProgramiv(_program, GL_LINK_STATUS, &status));
		if (!status)
		{
			char log[512];
			glCheck(glGetProgramInfoLog(_program, 512, NULL, log));
			Log(Error) << "Shader program link failed:\n" << log;

			glCheck(glDeleteProgram(_program));
			_program = 0;
		}

		Log(Debug) << "Program created: " << _filename;
	}

	void Shader::DestroyProgram()
	{
		if (_program)
		{
			glCheck(glDeleteProgram(_program));
			_program = 0;

			Log(Debug) << "Program destroyed: " << _filename;
		}
	}

	GLuint Shader::GetProgram()
	{
		return _program;
	}
}

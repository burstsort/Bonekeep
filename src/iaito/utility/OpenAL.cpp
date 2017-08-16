#include "iaito/utility/OpenAL.hpp"

#include "iaito/utility/Log.hpp"

namespace iaito
{
	void alCheckError(const std::string& file, unsigned int line)
	{
		ALenum code = alGetError();

		if (code != AL_NO_ERROR)
		{
			std::string(file);
			std::string error = "unknown error";

			switch (code)
			{
				case AL_INVALID_NAME:
					error = "AL_INVALID_ENUM";
					break;

				case AL_INVALID_ENUM:
					error = "AL_INVALID_VALUE";
					break;

				case AL_INVALID_VALUE:
					error = "AL_INVALID_OPERATION";
					break;

				case AL_INVALID_OPERATION:
					error = "AL_INVALID_FRAMEBUFFER_OPERATION";
					break;

				case AL_OUT_OF_MEMORY:
					error = "AL_OUT_OF_MEMORY";
					break;
			}

			Log(Error) 	<< "internal OpenAL error in " << file << " : " << line << " - " << error;
		}
	}
}
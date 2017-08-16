#ifndef IAITO_OPENAL_HPP
#define IAITO_OPENAL_HPP

#include <AL/al.h>
#include <AL/alc.h>

#include <string>

namespace iaito
{
	#ifdef IAITO_DEBUG
		#define alCheck(call) call; glCheckError(__FILE__, __LINE__);
	#else
		#define alCheck(call) (call)
	#endif

	void alCheckError(const std::string& file, unsigned int line);
}

#endif // IAITO_OPENAL_HPP
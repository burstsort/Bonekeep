#ifndef IAITO_AUDIO_HPP
#define IAITO_AUDIO_HPP

#include <string>
#include <vector>

#include "iaito/core/Resource.hpp"

namespace iaito
{
	class Audio: public Resource
	{
		public:
			Audio();

			void Load(const std::string& filename);
			void Unload();

			unsigned char* GetData();
			unsigned int GetSize();

		private:
			std::string _filename;
			unsigned char* _data;
			unsigned int _size;
	};
}

#endif // IAITO_AUDIO_HPP

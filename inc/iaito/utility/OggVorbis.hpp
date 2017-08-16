#ifndef IAITO_OGG_VORBIS_HPP
#define IAITO_OGG_VORBIS_HPP

#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>

namespace iaito
{
	struct ogg_file
	{
		unsigned char* curPtr;
		unsigned char* filePtr;
		size_t fileSize;
	};

	size_t readOgg(void* ptr, size_t size, size_t nmemb, void* datasource);
	int seekOgg(void* datasource, ogg_int64_t offset, int whence);
	int closeOgg(void* datasource);
	long tellOgg(void* datasource);
}

#endif // IAITO_OGG_VORBIS_HPP
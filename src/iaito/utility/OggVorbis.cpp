#include "iaito/utility/OggVorbis.hpp"

#include <cstring>

namespace iaito
{
	size_t readOgg(void* ptr, size_t size, size_t nmemb, void* datasource)
	{
		ogg_file* of = static_cast<ogg_file*>(datasource);
		size_t len = size * nmemb;
		if (of->curPtr + len > of->filePtr + of->fileSize)
			len = of->filePtr + of->fileSize - of->curPtr;

		memcpy(ptr, of->curPtr, len);
		of->curPtr += len;
		return len;
	}

	int seekOgg(void* datasource, ogg_int64_t offset, int whence)
	{
		ogg_file* of = static_cast<ogg_file*>(datasource);

		switch(whence)
		{
			case SEEK_CUR:
				of->curPtr += offset;
				break;
			case SEEK_END:
				of->curPtr = of->filePtr + of->fileSize - offset;
				break;
			case SEEK_SET:
				of->curPtr = of->filePtr + offset;
				break;
			default:
				return -1;
		}

		if (of->curPtr < of->filePtr)
		{
			of->curPtr = of->filePtr;
			return -1;
		}

		if (of->curPtr > of->filePtr + of->fileSize)
		{
			of->curPtr = of->filePtr + of->fileSize;
			return -1;
		}

		return 0;
	}

	int closeOgg(void* datasource)
	{
		return 0;
	}

	long tellOgg(void* datasource)
	{
		ogg_file* of = static_cast<ogg_file*>(datasource);
		return (of->curPtr - of->filePtr);
	}
}
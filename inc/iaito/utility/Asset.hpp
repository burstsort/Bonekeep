#ifndef IAITO_ASSET_HPP
#define IAITO_ASSET_HPP

#include <fstream>
#include <sstream>

#include <zlib.h>

#define FILENAME_LIMIT 32

namespace iaito
{
	class Asset
	{
		public:
			Asset(const std::string& filename);
			~Asset();

			std::istream& GetStream();

		private:
			Asset(const Asset& that);
			Asset& operator=(const Asset& that);

			std::stringstream _stream;
	};
}

#endif // IAITO_ASSET_HPP
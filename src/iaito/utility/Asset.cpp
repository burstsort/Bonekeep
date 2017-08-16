#include "iaito/utility/Asset.hpp"

#include "iaito/utility/Log.hpp"

namespace iaito
{
	Asset::Asset(const std::string& filename)
	{
		std::ifstream f;

		// Open the file
		f.open(filename.c_str(), std::ofstream::binary);

		// Read file
		if (f.good())
		{
			// Read directly from file
			_stream << f.rdbuf();
		}
		else
		{
			// Get archive and file names
			std::string file = filename.substr(filename.find("/") + 1);
			std::string archive = filename.substr(0, filename.find("/"));
			archive += ".ara";

			// Open the archive
			f.open(archive.c_str(), std::ifstream::binary);
			if (f.good())
			{
				// Get signature
				char sig[3];
				f.read(sig, 3);
				
				// Check signature
				if (sig[0] != 'A' || sig[1] != 'R' || sig[2] != 'A')
				{
					f.close();
					return;
				}

				// Get file count
				unsigned int n;
				f.read(reinterpret_cast<char*>(&n), sizeof(n));

				// Calculate offset
				unsigned long offset = 7 + n * 40 + 4;

				// Search for file in archive
				char fn[32];
				unsigned long step;
				bool found = false;
				for (size_t i = 0; i < n; ++i)
				{
					f.read(fn, FILENAME_LIMIT);

					if (fn == file)
					{
						// Found it
						found = true;
						break;
					}
					else
					{
						// Calculate offset
						f.seekg (4, f.cur);
						f.read(reinterpret_cast<char*>(&step), sizeof(step));
						offset += step;
					}
				}

				if (found)
				{
					// Read the file sizes
					unsigned long usize;
					unsigned long csize;
					f.read(reinterpret_cast<char*>(&usize), sizeof(usize));
					f.read(reinterpret_cast<char*>(&csize), sizeof(csize));

					// Go to file data
					f.seekg (offset, f.beg);

					// Read compressed data
					unsigned char* cdata = new unsigned char[csize];
					f.read(reinterpret_cast<char*>(cdata), csize);

					// Uncompress data
					unsigned char* udata = new unsigned char[usize];
					int res = uncompress(udata, &usize, cdata, csize);
					
					delete [] cdata;

					// Check compression result
					switch (res)
					{
						case Z_OK:
							break;
						case Z_MEM_ERROR:
							Log(Error) << "Out of memory!";
							break;
						case Z_BUF_ERROR:
							Log(Error) << "Buffer too small!";
							break;
						case Z_DATA_ERROR:
							Log(Error) << "Corrupted data!";
							break;
					}

					// Write file to stream
					_stream.write(reinterpret_cast<char*>(udata), usize);
					
					delete [] udata;
				}
				else
					// File not found
					Log(Error) << "Unable to load asset " << file << " from " << archive << "!";
			}
		}

		// Close the file
		f.close();
	}

	Asset::~Asset()
	{

	}

	std::istream& Asset::GetStream()
	{
		return _stream;
	}
}
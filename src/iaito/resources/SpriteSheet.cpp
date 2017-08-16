#include "iaito/resources/SpriteSheet.hpp"

#include <fstream>
#include "iaito/utility/Log.hpp"

#include "iaito/utility/Asset.hpp"

namespace iaito
{
	SpriteSheet::SpriteSheet() : _filename(""), _texture(0), _width(0), _height(0) {}

	void SpriteSheet::Load(const std::string& filename)
	{
		Unload();

		// Store filename
		_filename = filename;

		Log(Debug) << "Loading spritesheet: " << _filename << ".spr";

		// Read frame data
		Asset aspr(_filename+".spr");
		std::istream& spr = aspr.GetStream();

		if (!spr.good())
		{
			Log(Error) << "Error loading sprite data from " << _filename << ".spr";
			return;
		}

		std::string str;
		unsigned int n;
		unsigned int index;

		while (spr.good())
		{
			if (!str.empty())
			{
				index = _sets.size();
				_sets[str] = index;
				spr >> n; _sizesX.push_back(n);
				spr >> n; _sizesY.push_back(n);
				spr >> n; _pivotsX.push_back(n);
				spr >> n; _pivotsY.push_back(n);

				spr >> n; _count.push_back(n);
				for (unsigned int f = n; f > 0; f--)
				{
					spr >> n; _framesX[index].push_back(n);
					spr >> n; _framesY[index].push_back(n);
				}
			}
			spr >> str;
		}

		CreateTexture();
	}

	void SpriteSheet::Unload()
	{
		if (_filename.empty())
			return;

		Log(Debug) << "Unloading spritesheet: " << _filename << ".spr";

		_width = 0;
		_height = 0;

		// Clear memory
		_sets.clear();
		_count.clear();
		_sizesX.clear();
		_sizesY.clear();
		_pivotsX.clear();
		_pivotsY.clear();
		_framesX.clear();
		_framesY.clear();

		// Reallocate memory
		_sets.swap(_sets);
		_count.swap(_count);
		_sizesX.swap(_sizesX);
		_sizesY.swap(_sizesY);
		_pivotsX.swap(_pivotsX);
		_pivotsY.swap(_pivotsY);
		_framesX.swap(_framesX);
		_framesY.swap(_framesY);

		DestroyTexture();
		_filename.clear();
	}

	void SpriteSheet::CreateTexture()
	{
		if (_filename.empty() || _texture)
			return;

		// Open a TGA file
		Asset atga(_filename+".tga");
		std::istream& tga = atga.GetStream();
		
		if (tga.good())
		{
			// Ignore first two bytes
			tga.ignore(2);

			// Read image type
			char type;
			tga.read(&type, 1);
			
			if (type == 2 || type == 10)
			{
				// Read image specification
				tga.ignore(9);
				tga.read(reinterpret_cast<char*>(&_width), 2);
				tga.read(reinterpret_cast<char*>(&_height), 2);
				char bitDepth;
				tga.read(reinterpret_cast<char*>(&bitDepth), 1);

				tga.ignore(1);

				if (bitDepth == 24 || bitDepth == 32)
				{
					int colorMode = bitDepth/8;
					int imageSize = _width*_height*colorMode;

					unsigned char *data = new unsigned char[imageSize];

					if (type == 2)
						tga.read(reinterpret_cast<char*>(data), imageSize);
					else if (type == 10)
					{
						int count = _width*_height;
						int pixel = 0;
						int byte = 0;
						unsigned char* buffer = new unsigned char[colorMode];

						// Read compressed data
						while(pixel < count)
						{
							unsigned char header = 0;
							tga.read(reinterpret_cast<char*>(&header), 1);
							if (header<128)
							{
								// RAW
								header++;
								for (unsigned int i = 0; i < header; ++i)
								{
									tga.read(reinterpret_cast<char*>(buffer), colorMode);
									data[byte] = buffer[0];
									data[byte+1] = buffer[1];
									data[byte+2] = buffer[2];
									if (colorMode == 4)
										data[byte+3] = buffer[3];

									byte += colorMode;
									pixel++;
								}
							}
							else
							{
								// RLE
								header -= 127;
								tga.read(reinterpret_cast<char*>(buffer), colorMode);
								for (unsigned int i = 0; i < header; ++i)
								{
									data[byte] = buffer[0];
									data[byte+1] = buffer[1];
									data[byte+2] = buffer[2];
									if (colorMode == 4)
										data[byte+3] = buffer[3];

									byte += colorMode;
									pixel++;
								}
							}
						}
						delete[] buffer;

						// Create OpenGL texture
						glCheck(glGenTextures(1, &_texture));
						glCheck(glBindTexture(GL_TEXTURE_2D, _texture));

						if (bitDepth == 24)
							glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, data));
						else if (bitDepth == 32)
							glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data));

						// Set texture parameters
						glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
						glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
						glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
						glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
					}

					delete[] data;
				}
				else
					Log(Error) << _filename << ".tga is not 24-bit or 32-bit image";
			}
			else
				Log(Error) << _filename << ".tga is not a true-color image type";
		}
		else
			Log(Error) << "failed to open " << _filename << ".tga";

		Log(Debug) << "Texture created: " << _filename<< ".tga";
	}

	void SpriteSheet::DestroyTexture()
	{
		if (_texture)
		{
			glCheck(glDeleteTextures(1, &_texture));
			_texture = 0;

			Log(Debug) << "Texture destroyed: " << _filename<< ".tga";
		}
	}

	GLuint SpriteSheet::GetTexture()
	{
		return _texture;
	}

	unsigned int SpriteSheet::GetWidth()
	{
		return _width;
	}

	unsigned int SpriteSheet::GetHeight()
	{
		return _height;
	}

	unsigned int SpriteSheet::GetSet(const std::string& name)
	{
		if (_sets.count(name))
			return _sets[name];

			return 0;
	}

	unsigned int SpriteSheet::GetCount(unsigned int set)
	{
		if (set < _count.size())
			return _count[set];

		return 0;
	}

	unsigned int SpriteSheet::GetSizeX(unsigned int set)
	{
		if (set < _sizesX.size())
			return _sizesX[set];

		return 0;
	}

	unsigned int SpriteSheet::GetSizeY(unsigned int set)
	{
		if (set < _sizesY.size())
			return _sizesY[set];

		return 0;
	}

	unsigned int SpriteSheet::GetPivotX(unsigned int set)
	{
		if (set < _pivotsX.size())
			return _pivotsX[set];

		return 0;
	}

	unsigned int SpriteSheet::GetPivotY(unsigned int set)
	{
		if (set < _pivotsY.size())
			return _pivotsY[set];

		return 0;
	}

	unsigned int SpriteSheet::GetOffsetX(unsigned int set, unsigned int frame)
	{
		if (_framesX.count(set))
			return _framesX[set][frame % _framesX[set].size()];

		return 0;
	}

	unsigned int SpriteSheet::GetOffsetY(unsigned int set, unsigned int frame)
	{
		if (_framesY.count(set))
			return _framesY[set][frame % _framesY[set].size()];

		return 0;
	}
}

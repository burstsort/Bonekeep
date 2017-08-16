#include "iaito/resources/Audio.hpp"

#include "iaito/utility/Log.hpp"

namespace iaito
{
	Audio::Audio() : _data(NULL), _size(0){}

	void Audio::Load(const std::string& filename)
	{
		Unload();

		// Store filename
		_filename = filename;

		Log(Debug) << "Loading sound: " << _filename;

		std::ifstream file((_filename+".ogg").c_str(), std::ifstream::binary);

		if (file.good())
		{
			file.seekg(0, std::ios::end);
			_size = file.tellg();
			file.seekg(0, std::ios::beg);

			_data = new unsigned char[_size];
			
			file.read(reinterpret_cast<char*>(_data), _size);
		}
	}

	void Audio::Unload()
	{
		if (_data)
		{
			Log(Debug) << "Unloading sound: " << _filename;

			delete [] _data;
			_data = NULL;
		}
		
		_size = 0;
		_filename.clear();
	}

	unsigned char* Audio::GetData()
	{
		return &_data[0];
	}

	unsigned int Audio::GetSize()
	{
		return _size;
	}
}
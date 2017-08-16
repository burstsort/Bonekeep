#include "iaito/resources/Collider.hpp"

#include <fstream>
#include "iaito/utility/Log.hpp"

#include "iaito/utility/Asset.hpp"

namespace iaito
{
	Collider::Collider() : _filename("") {}

	void Collider::Load(const std::string& filename)
	{
		Unload();

		// Store filename
		_filename = filename;

		Log(Debug) << "Loading collider: " << _filename << ".col";

		// Read frame data
		Asset aspr(_filename+".col");
		std::istream& spr = aspr.GetStream();

		if (!spr.good())
		{
			Log(Error) << "Error loading collider data from " << _filename << ".col";
			return;
		}

		std::string str;

		while (spr.good())
		{
			if (!str.empty())
			{
				
				// TO DO

			}
			spr >> str;
		}

	}

	void Collider::Unload()
	{
		if (_filename.empty())
			return;

		Log(Debug) << "Unloading collider: " << _filename << ".col";

		// TO DO

		_filename.clear();
	}
}

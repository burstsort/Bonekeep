#ifndef IAITO_COLLIDER_HPP
#define IAITO_COLLIDER_HPP

#include <string>
#include <vector>
#include <map>

#include "iaito/core/Resource.hpp"

namespace iaito
{
	class Collider: public Resource
	{
		public:
			Collider();
			
			void Load(const std::string& filename);
			void Unload();

			unsigned int GetWidth();
			unsigned int GetHeight();
			unsigned int GetSet(const std::string& name);

			unsigned int GetCount(unsigned int set);
			unsigned int GetSizeX(unsigned int set);
			unsigned int GetSizeY(unsigned int set);
			unsigned int GetPivotX(unsigned int set);
			unsigned int GetPivotY(unsigned int set);
			unsigned int GetOffsetX(unsigned int set, unsigned int frame);
			unsigned int GetOffsetY(unsigned int set, unsigned int frame);

		private:
			std::string _filename;

			std::map<std::string, unsigned int> _sets;

			std::vector<unsigned int> _count;
	};
}

#endif // IAITO_COLLIDER_HPP
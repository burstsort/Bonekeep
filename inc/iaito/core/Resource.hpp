#ifndef IAITO_RESOURCE_HPP
#define IAITO_RESOURCE_HPP

#include <string>

namespace iaito
{
	class Resource
	{
		public:
			virtual ~Resource(){};

			virtual void Load(const std::string& filename){};
			virtual void Unload(){};
	};
}

#endif // IAITO_RESOURCE_HPP
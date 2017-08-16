#ifndef IAITO_ENTITY_MANAGER_HPP
#define IAITO_ENTITY_MANAGER_HPP

#include <typeinfo>
#include <vector>
#include <map>

#include "iaito/core/Service.hpp"
#include "iaito/core/Components.hpp"
#include "iaito/utility/Log.hpp"

namespace iaito
{
	class EntityManager: public Service
	{
		public:
			EntityManager();
			~EntityManager();

			// Entity management
			unsigned int Create();
			void Destroy(unsigned int index);
			void Clear();
			unsigned int Size();

			unsigned long GetId(unsigned int index);
			unsigned int GetIndex(unsigned long id);

			void Enable(unsigned int index, int mask);
			void Disable(unsigned int index, int mask);
			bool Check(unsigned int index, int mask);

			void Tag(unsigned int index, int tags);
			void Untag(unsigned int index, int tags);
			bool CheckTag(unsigned int index, int tags);

			// Component registration
			template <typename T>
			void Register()
			{
				// Check if already registered
				if (_map.count(&typeid(T)))
						return;

				// Register
				_types.push_back(&typeid(T));
				_map[&typeid(T)] = new Components<T>;

				// Add components for existing entities
				for (unsigned int i = 0; i < _ids.size(); ++i)
					_map[&typeid(T)]->Add();

				Log(Debug)<<typeid(T).name()<<" components registered";
			}

			// Component access
			template <typename T>
			Components<T>& Component()
			{
				// Check if exists
				if (!_map.count(&typeid(T)))
				{
					Log(Error)<<"Trying to access unregistered components";
					Register<T>();
				}

				return *dynamic_cast<Components<T>*>(_map[&typeid(T)]);
			}

		private:
			EntityManager(const EntityManager& that);
			EntityManager& operator=(const EntityManager& that);

			std::vector<const std::type_info*> _types;
			std::map<const std::type_info*, ComponentsBase*> _map;

			std::vector<unsigned int> _empty;
			std::vector<int> _masks;
			std::vector<int> _tags;
			
			std::vector<unsigned long> _ids;
			unsigned long _nextId;
	};
}

#endif // IAITO_ENTITY_MANAGER_HPP
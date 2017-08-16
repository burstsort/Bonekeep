#ifndef IAITO_GAME_HPP
#define IAITO_GAME_HPP

#include <typeinfo>
#include <vector>
#include <map>

#include "iaito/Context.hpp"
#include "iaito/core/Service.hpp"
#include "iaito/utility/Log.hpp"

namespace iaito
{
	class Game
	{
		public:
			Game();
			~Game();

			void Start();
			void Stop();

			Context& GetContext();

			// Service registration
			template <typename T>
			void Register()
			{
				// Check if already registered
				if (_map.count(&typeid(T)))
					return;

				// Register
				_types.push_back(&typeid(T));
				_map[&typeid(T)] = new T;

				Log(Debug)<<typeid(T).name()<<" service registered";
			}

			// Service access
			template <typename T>
			T& Get()
			{
				// Check if exists
				if (!_map.count(&typeid(T)))
				{
					Log(Error)<<"Trying to access unregistered service";
					Register<T>();
				}

				return *dynamic_cast<T*>(_map[&typeid(T)]);
			}

			// System registration
			template <void (*T)(Game&)>
			void Register()
			{
				_systems.push_back(T);
			}

		private:
			Game(const Game& that);
			Game& operator=(const Game& that);

			Context _context;
			
			std::vector<const std::type_info*> _types;
			std::map<const std::type_info*, Service*> _map;

			std::vector<void (*)(Game&)> _systems;

			bool _running;
	};
}

#endif // IAITO_GAME_HPP
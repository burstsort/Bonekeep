#include "iaito/Game.hpp"

namespace iaito
{
	Game::Game()
	{
		_running = false;
	}

	Game::~Game()
	{
		// Unregister services
		while (_types.size())
		{
			delete _map[_types[_types.size()-1]];
			_types.pop_back();
		}
	}

	void Game::Start()
	{
		_running = true;

		Log(Info)<<"Game started";
		Log(Debug)<<_types.size()<<" services and "<<_systems.size()<<" systems registered";

		// Update services and systems
		while (_running)
		{
			for (unsigned int i = 0; i < _types.size(); ++i)
				_map[_types[i]]->Update();

			for (unsigned int i = 0; i < _systems.size(); ++i)
				_systems[i](*this);

			_context.Update();
		}
	}

	void Game::Stop()
	{
		Log(Info)<<"Game stopped";

		_running = false;
	}

	Context& Game::GetContext()
	{
		return _context;
	}
}

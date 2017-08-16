#ifndef BONEKEEP_SPAWNER_HPP
#define BONEKEEP_SPAWNER_HPP

#include "iaito/Game.hpp"

using namespace iaito;

namespace bonekeep
{
	unsigned int SpawnScenery(Game& game);
	unsigned int SpawnPlayer(Game& game);
	unsigned int SpawnPickup(Game& game);
	unsigned int SpawnEnemy(Game& game, float x, float y);
}

#endif // BONEKEEP_SPAWNER_HPP
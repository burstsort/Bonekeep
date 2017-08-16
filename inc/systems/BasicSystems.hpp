#ifndef BONEKEEP_BASIC_SYSTEMS_HPP
#define BONEKEEP_BASIC_SYSTEMS_HPP

#include "iaito/Game.hpp"

using namespace iaito;

namespace bonekeep
{
	void FPS(Game& game);
	void ESC(Game& game);

	void Animate(Game& game);
	void Bounce(Game& game);

	void Render(Game& game);
	void Controller(Game& game);

	void PickupItem(Game& game);
	void CheckPlayerDeath(Game& game);

	void TestCollisions(Game& game);
}

#endif // BONEKEEP_BASIC_SYSTEMS_HPP
#include "Spawners.hpp"

#include "iaito/Iaito.hpp"
#include "Masks.hpp"
#include "components/BasicComponents.hpp"

	// Needed for rand()
	#include <cstdlib>

using namespace iaito;

namespace bonekeep
{
	unsigned int SpawnScenery(Game& game)
	{
		EntityManager& em = game.Get<EntityManager>();
		unsigned int i = em.Create();

		Components<Position>& c_pos = em.Component<Position>();
		c_pos.Get(i).x = 0.0f;
		c_pos.Get(i).y = 0.0f;
		c_pos.Get(i).depth = 200.0f;

		Components<Sprite>& c_spr = em.Component<Sprite>();
		c_spr.Get(i).spriteSheet = game.Get<SpriteRenderer>().BindSpriteSheet("sprites/bg");
		c_spr.Get(i).spriteIndex = 0;
		c_spr.Get(i).hScale = 1.0f;
		c_spr.Get(i).vScale = 1.0f;
		c_spr.Get(i).frameIndex = 0;
		c_spr.Get(i).frameSpeed = 0.0f;

		em.Enable(i, POSITION | SPRITE);

		return i;
	}

	unsigned int SpawnPlayer(Game& game)
	{
		EntityManager& em = game.Get<EntityManager>();
	 	unsigned int i = em.Create();

		Components<Position>& c_pos = em.Component<Position>();
		c_pos.Get(i).x = 640 * 0.5f;
		c_pos.Get(i).y = 480 * 0.5f;
		c_pos.Get(i).depth = 80.0f;

		Components<Sprite>& c_spr = em.Component<Sprite>();
		c_spr.Get(i).spriteSheet = game.Get<SpriteRenderer>().BindSpriteSheet("sprites/player");
		c_spr.Get(i).spriteIndex = 0;
		c_spr.Get(i).hScale = 1.0f;
		c_spr.Get(i).vScale = 1.0f;
		c_spr.Get(i).frameIndex = 0;
		c_spr.Get(i).frameSpeed = 0.0f;

		Components<Control>& c_con = em.Component<Control>();
		c_con.Get(i).speed = 100.0f;

		Components<Collision>& c_col = em.Component<Collision>();
		c_col.Get(i).xMin = -4.0f;
		c_col.Get(i).yMin = -28.0f;
		c_col.Get(i).xMax = 4.0f;
		c_col.Get(i).yMax = 0.0f;

		em.Tag(i, PLAYER);
		em.Enable(i, POSITION | SPRITE | CONTROL | COLLISION);

		return i;
	}

	unsigned int SpawnPickup(Game& game)
	{
		EntityManager& em = game.Get<EntityManager>();
		unsigned int i = em.Create();

		Components<Position>& c_pos = em.Component<Position>();
		c_pos.Get(i).x = 40.0f + 560.0f * rand()/(RAND_MAX+1.0f);
		c_pos.Get(i).y = 40.0f + 400.0f * rand()/(RAND_MAX+1.0f);
		c_pos.Get(i).depth = 40.0f;

		Components<Sprite>& c_spr = em.Component<Sprite>();
		c_spr.Get(i).spriteSheet = game.Get<SpriteRenderer>().BindSpriteSheet("sprites/food");
		c_spr.Get(i).spriteIndex = 0;
		c_spr.Get(i).hScale = 1.0f;
		c_spr.Get(i).vScale = 1.0f;
		c_spr.Get(i).frameIndex = 0;
		c_spr.Get(i).frameSpeed = 0.0f;

		Components<Collision>& c_col = em.Component<Collision>();
		c_col.Get(i).xMin = -8.0f;
		c_col.Get(i).yMin = -8.0f;
		c_col.Get(i).xMax = 8.0f;
		c_col.Get(i).yMax = 0.0f;

		em.Tag(i, PICKUP);
		em.Enable(i, POSITION | SPRITE | COLLISION);

		return i;
	}

	unsigned int SpawnEnemy(Game& game, float x, float y)
	{
		EntityManager& em = game.Get<EntityManager>();
		unsigned int i = em.Create();

		Components<Position>& c_pos = em.Component<Position>();
		c_pos.Get(i).x = x;
		c_pos.Get(i).y = y;
		c_pos.Get(i).depth = 40.0f;

		Components<Physics>& c_phy = em.Component<Physics>();
		c_phy.Get(i).hSpeed = 20.0f + 40.0f * rand()/(RAND_MAX+1.0f);
		c_phy.Get(i).vSpeed = 10.0f + 20.0f * rand()/(RAND_MAX+1.0f);

		Components<Sprite>& c_spr = em.Component<Sprite>();
		c_spr.Get(i).spriteSheet = game.Get<SpriteRenderer>().BindSpriteSheet("sprites/skeleton");
		c_spr.Get(i).spriteIndex = 0;
		c_spr.Get(i).hScale = 1.0f;
		c_spr.Get(i).vScale = 1.0f;
		c_spr.Get(i).frameIndex = 0;
		c_spr.Get(i).frameSpeed = c_phy.Get(i).hSpeed / 10.0f;

		Components<Collision>& c_col = em.Component<Collision>();
		c_col.Get(i).xMin = -4.0f;
		c_col.Get(i).yMin = -28.0f;
		c_col.Get(i).xMax = 4.0f;
		c_col.Get(i).yMax = 0.0f;

		em.Tag(i, ENEMY);
		em.Enable(i, POSITION | PHYSICS | SPRITE | COLLISION);

		return i;
	}
}
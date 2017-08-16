#include "systems/BasicSystems.hpp"

#include "iaito/Iaito.hpp"
#include "Masks.hpp"
#include "components/BasicComponents.hpp"

#include "Spawners.hpp"

#include <iostream>
#include <sstream>

	// Needed for rand()
	#include <cstdlib>

	// Needed for trig and abs
	#include <cmath>

using namespace iaito;

namespace bonekeep
{
	// Display FPS in window title bar
	void FPS(Game& game)
	{
		Context& win = game.GetContext();

		std::ostringstream title;
		title<<"FPS: "<<static_cast<int>(1/game.GetContext().GetTimeDelta());
		win.SetTitle(title.str());
	}

	// Quit the game when windows is closed or ESC key pressed
	void ESC(Game& game)
	{
		Context& win = game.GetContext();

		if (win.IsClosed() || win.CheckKeyPress(KeyEscape))
			game.Stop();
	}

	// Animate the sprites
	void Animate(Game& game)
	{
		Context& c = game.GetContext();

		EntityManager& em = game.Get<EntityManager>();
		Components<Sprite>& c_spr = em.Component<Sprite>();

		for (unsigned int i = 0; i < em.Size(); ++i)
		{
			if (em.Check(i, SPRITE))
			{
				Sprite& spr = c_spr.Get(i);
				spr.frameIndex += spr.frameSpeed * c.GetTimeDelta();
			}
		}
	}

	// Make sure that entities with physics dont leave the screen
	void Bounce(Game& game)
	{
		Context& c = game.GetContext();

		EntityManager& em = game.Get<EntityManager>();
		Components<Position>& c_pos = em.Component<Position>();
		Components<Physics>& c_phy = em.Component<Physics>();
		Components<Sprite>& c_spr = em.Component<Sprite>();

		int margin = 20;
		unsigned int w, h;
		float scale;
		game.Get<ViewManager>().GetSize(&w, &h);
		game.Get<ViewManager>().GetScale(&scale);
		scale = 1/scale;

		for (unsigned int i = 0; i < em.Size(); ++i)
		{
			if (em.Check(i, POSITION | PHYSICS))
			{
				Position& p = c_pos.Get(i);
				Physics& s = c_phy.Get(i);

				if ((p.x > w * scale - margin && s.hSpeed > 0) || (p.x < margin && s.hSpeed < 0))
					s.hSpeed *= -1;

				if ((p.y > h * scale - margin && s.vSpeed > 0) || (p.y < margin && s.vSpeed < 0))
					s.vSpeed *= -1;

				p.x += s.hSpeed * c.GetTimeDelta();
				p.y += s.vSpeed * c.GetTimeDelta();

				if (em.Check(i,SPRITE))
				{
					if (s.hSpeed < 0)
						c_spr.Get(i).hScale = 1;
					else
						c_spr.Get(i).hScale = -1;
				}
			}
		}
	}

	// Render sprites on screen, also render collision debug information if mouse is within the window
	void Render(Game& game)
	{
		EntityManager& em = game.Get<EntityManager>();
		Components<Position>& c_pos = em.Component<Position>();
		Components<Sprite>& c_spr = em.Component<Sprite>();
		Components<Collision>& c_col = em.Component<Collision>();

		SpriteRenderer& r = game.Get<SpriteRenderer>();
		DebugRenderer& d = game.Get<DebugRenderer>();
		ViewManager& v = game.Get<ViewManager>();

		float vx, vy, scale;
		v.GetPosition(&vx, &vy);
		v.GetScale(&scale);

		unsigned int vw, vh;
		v.GetSize(&vw, &vh);

		r.Clear();
		d.Clear();

		// Render sprites
		for (unsigned int i = 0; i < em.Size(); ++i)
		{
			if (em.Check(i, POSITION | SPRITE))
			{
				r.SetSpriteSheet(c_spr.Get(i).spriteSheet);

				Position p = c_pos.Get(i);
				p.x = (p.x - vx) * 2.0f * scale / vw; // (int)
				p.y = (p.y - vy) * 2.0f * scale / vh; // (int)
				float scalex = c_spr.Get(i).hScale * 2.0f * scale / vw;
				float scaley = c_spr.Get(i).vScale * 2.0f * scale / vh;

				r.SetSpritePosition(p.x, p.y, p.depth * 0.001f);
				r.SetSpriteTransform(scalex, scaley, c_spr.Get(i).angle);
				r.SetSpriteColor(c_spr.Get(i).r, c_spr.Get(i).g, c_spr.Get(i).b);

				r.DrawSprite(c_spr.Get(i).spriteIndex, c_spr.Get(i).frameIndex);
			}
		}

		r.Render();

		int mx, my;
		game.GetContext().GetMousePosition(&mx, &my);

		if (mx > 0 && my > 0 && mx < 640 && my < 480)
		{
			// Render grid
			d.SetColor(0.4f, 0.0f, 0.0f);
			for (unsigned int y = 0; y < 8; ++y)
			{
				for (unsigned int x = 0; x < 10; ++x)
				{
					float xLU = (x*64 - vx) * 2.0f * scale / vw;
					float yLU = (y*64 - vy) * 2.0f * scale / vh;
					float xRD = (x*64+64 - vx) * 2.0f * scale / vw;
					float yRD = (y*64+64 - vy) * 2.0f * scale / vh;

					d.DrawRectangle(xLU, yLU, xRD, yRD);
				}
			}

			// Render mouse collision
			d.SetColor(0.8f, 0.2f, 0.2f);
			d.DrawRectangle((mx/scale - 8.0f) * 2.0f * scale / vw, (my/scale - 8.0f) * 2.0f * scale / vh, (mx/scale + 8.0f) * 2.0f * scale / vw, (my/scale + 8.0f) * 2.0f * scale / vh);

			// Render entity collisions
			d.SetColor(0.8f, 0.2f, 0.2f);
			for (unsigned int i = 0; i < em.Size(); ++i)
			{
				if (em.Check(i, POSITION))
				{
					Position p = c_pos.Get(i);
					
					float xLU = (p.x+c_col.Get(i).xMin - vx) * 2.0f * scale / vw; 
					float yLU = (p.y+c_col.Get(i).yMin - vy) * 2.0f * scale / vh;

					float xRD = (p.x+c_col.Get(i).xMax - vx) * 2.0f * scale / vw; 
					float yRD = (p.y+c_col.Get(i).yMax - vy) * 2.0f * scale / vh;

					d.DrawRectangle(xLU, yLU, xRD, yRD);
				}
			}

			d.Render();
		}
	}

	// Allow keyboard control
	void Controller(Game& game)
	{
		Context& context = game.GetContext();

		EntityManager& em = game.Get<EntityManager>();
		Components<Position>& c_pos = em.Component<Position>();
		Components<Control>& c_con = em.Component<Control>();
		Components<Sprite>& c_spr = em.Component<Sprite>();

		for (unsigned int i = 0; i < em.Size(); ++i)
		{
			if (em.Check(i, POSITION | CONTROL))
			{
				Position& p = c_pos.Get(i);
				Control& c = c_con.Get(i);

				float dir_x = 0;
				float dir_y = 0;

				if (context.CheckKeyHold(KeyUp)) dir_y = -1.0f;
				if (context.CheckKeyHold(KeyLeft)) dir_x = -1.0f;
				if (context.CheckKeyHold(KeyDown)) dir_y = 1.0f;
				if (context.CheckKeyHold(KeyRight)) dir_x = 1.0f;

				p.x += dir_x * c.speed * context.GetTimeDelta();
				p.y += dir_y * c.speed * context.GetTimeDelta();

				// Limit movement by screen borders
				if (p.x < 24) p.x = 24;
				if (p.x > 616) p.x = 616;
				if (p.y < 24) p.y = 24;
				if (p.y > 464) p.y = 464;

				if (em.CheckTag(i, PLAYER) && em.Check(i, SPRITE))
				{
					// Toggle animation
					if (dir_x != 0 || dir_y != 0)
						c_spr.Get(i).frameSpeed = 10.0f;
					else
						c_spr.Get(i).frameSpeed = 0.0f;

					// Switch sprite direction
					if (dir_x < 0)
						c_spr.Get(i).hScale = 1.0f;
					else if (dir_x > 0)
						c_spr.Get(i).hScale = -1.0f;
				}
			}
		}
	}

	// Pickups can be collected by player
	void PickupItem(Game& game)
	{
		CollisionManager& cm = game.Get<CollisionManager>();
		EntityManager& em = game.Get<EntityManager>();

		Components<Position>& c_pos = em.Component<Position>();
		Components<Collision>& c_col = em.Component<Collision>();

		cm.Clear();

		// Add player entities to the collision check
		for (unsigned int i = 0; i < em.Size(); ++i)
		{
			if (em.Check(i, POSITION | COLLISION) && em.CheckTag(i, PLAYER))
			{
				Position& pos = c_pos.Get(i);
				Collision& col = c_col.Get(i);

				cm.Insert(pos.x+col.xMin, pos.y+col.yMin, pos.x+col.xMax, pos.y+col.yMax, i);
			}
		}

		float enemy_x = 0.0f;
		float enemy_y = 0.0f;
		bool enemy_spawn = false;

		for (unsigned int i = 0; i < em.Size(); ++i)
		{
			if (em.Check(i, POSITION | COLLISION) && em.CheckTag(i, PICKUP))
			{
				Position& my_pos = c_pos.Get(i);
				Collision& my_col = c_col.Get(i);

				// Bread phase collision
				std::vector<unsigned int> nearby = cm.Query(my_pos.x+my_col.xMin, my_pos.y+my_col.yMin, my_pos.x+my_col.xMax, my_pos.y+my_col.yMax);

				// Go over entities with potential collision
				for (unsigned int j = 0; j < nearby.size(); ++j)
				{
					if (em.Check(nearby[j], POSITION | CONTROL | COLLISION))
					{
						Position& pos = c_pos.Get(nearby[j]);
						Collision& col = c_col.Get(nearby[j]);

						// Check if actually colliding
						if (cm.CheckCollision(my_pos.x+my_col.xMin, my_pos.y+my_col.yMin, my_pos.x+my_col.xMax, my_pos.y+my_col.yMax, pos.x+col.xMin, pos.y+col.yMin, pos.x+col.xMax, pos.y+col.yMax))
						{
							// Set a flag to spawn an new enemy
							enemy_x = (my_pos.x > 320.0f) ? -20.0f : 660.0f;
							enemy_y = my_pos.y;
							enemy_spawn = true;

							// Move to a new random position
							my_pos.x = 40.0f + 560.0f * rand()/(RAND_MAX+1.0f);
							my_pos.y = 40.0f + 400.0f * rand()/(RAND_MAX+1.0f);

							// Increase the score
							em.Component<Control>().Get(nearby[j]).score += 1.0f;
							std::cout << "Score: " << em.Component<Control>().Get(nearby[j]).score << std::endl;
							game.Get<SoundManager>().Play(1);
						}
					}
				}
			}
		}

		if (enemy_spawn)
		{
			SpawnEnemy(game, enemy_x, enemy_y);
			game.Get<SoundManager>().Play(2);
		}
	}

	// Check player death conditions
	void CheckPlayerDeath(Game& game)
	{
		CollisionManager& cm = game.Get<CollisionManager>();
		EntityManager& em = game.Get<EntityManager>();

		Components<Position>& c_pos = em.Component<Position>();
		Components<Collision>& c_col = em.Component<Collision>();

		cm.Clear();

		// Add enemies to collision check
		for (unsigned int i = 0; i < em.Size(); ++i)
		{
			if (em.Check(i, POSITION | COLLISION) && em.CheckTag(i, ENEMY))
			{
				Position& pos = c_pos.Get(i);
				Collision& col = c_col.Get(i);

				cm.Insert(pos.x+col.xMin, pos.y+col.yMin, pos.x+col.xMax, pos.y+col.yMax, i);
			}
		}

		for (unsigned int i = 0; i < em.Size(); ++i)
		{
			if (em.Check(i, POSITION | CONTROL | COLLISION) && em.CheckTag(i, PLAYER))
			{
				Position& my_pos = c_pos.Get(i);
				Collision& my_col = c_col.Get(i);

				// Broad phase collision
				std::vector<unsigned int> nearby = cm.Query(my_pos.x+my_col.xMin, my_pos.y+my_col.yMin, my_pos.x+my_col.xMax, my_pos.y+my_col.yMax);

				// Go over entities with potential collision
				for (unsigned int j = 0; j < nearby.size(); ++j)
				{
					if (em.Check(nearby[j], POSITION | COLLISION))
					{
						Position& pos = c_pos.Get(nearby[j]);
						Collision& col = c_col.Get(nearby[j]);

						// Check if actually colliding
						if (cm.CheckCollision(my_pos.x+my_col.xMin, my_pos.y+my_col.yMin, my_pos.x+my_col.xMax, my_pos.y+my_col.yMax, pos.x+col.xMin, pos.y+col.yMin, pos.x+col.xMax, pos.y+col.yMax))
						{
							std::cout << "GAME OVER! Score: " << em.Component<Control>().Get(i).score << std::endl;
							game.Get<SoundManager>().Play(3);
							
							// Reset Game
							em.Clear();
							SpawnScenery(game);
							SpawnPlayer(game);
							SpawnPickup(game);
							SpawnEnemy(game, 40.0f, 40.0f);
							SpawnEnemy(game, 40.0f, 440.0f);
							SpawnEnemy(game, 600.0f, 40.0f);
							SpawnEnemy(game, 600.0f, 440.0f);
						}
					}
				}
			}
		}
	}

	// Change sprite color depending on mouse collision
	void TestCollisions(Game& game)
	{
		Context& c = game.GetContext();

		CollisionManager& cm = game.Get<CollisionManager>();
		EntityManager& em = game.Get<EntityManager>();
		ViewManager& vm = game.Get<ViewManager>();

		Components<Position>& c_pos = em.Component<Position>();
		Components<Collision>& c_col = em.Component<Collision>();
		Components<Sprite>& c_spr = em.Component<Sprite>();

		cm.Clear();

		int mx, my;
		c.GetMousePosition(&mx, &my);

		float vx, vy, scale;
		vm.GetPosition(&vx, &vy);
		vm.GetScale(&scale);

		Position test_pos;
		test_pos.x = mx / scale + vx;
		test_pos.y = my / scale + vy;

		// Add entities to collision check
		for (unsigned int i = 0; i < em.Size(); ++i)
		{
			Position& pos = c_pos.Get(i);
			Collision& col = c_col.Get(i);

			cm.Insert(pos.x+col.xMin, pos.y+col.yMin, pos.x+col.xMax, pos.y+col.yMax, i);

			c_spr.Get(i).spriteIndex = 0;
		}

		// Broad phase collision
		std::vector<unsigned int> nearby = cm.Query(test_pos.x-8.0f, test_pos.y-8.0f, test_pos.x+8.0f, test_pos.y+8.0f);

		// Go over entities with potential collision
		for (unsigned int i = 0; i < nearby.size(); ++i)
		{
			if (!em.CheckTag(nearby[i], PICKUP) && !em.CheckTag(nearby[i], SCENERY))
			{
				c_spr.Get(nearby[i]).spriteIndex = 1;

				Position& pos = c_pos.Get(nearby[i]);
				Collision& col = c_col.Get(nearby[i]);

				// Check if actually colliding
				if (cm.CheckCollision(test_pos.x-8.0f, test_pos.y-8.0f, test_pos.x+8.0f, test_pos.y+8.0f, pos.x+col.xMin, pos.y+col.yMin, pos.x+col.xMax, pos.y+col.yMax))
						c_spr.Get(nearby[i]).spriteIndex = 2;
			}
		}
	}
}
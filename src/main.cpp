#include <iostream>

	// Needed for rand()
	#include <cstdlib>
	#include <ctime>

#include "iaito/Iaito.hpp"

#include "Masks.hpp"
#include "Spawners.hpp"
#include "components/BasicComponents.hpp"
#include "systems/BasicSystems.hpp"

#define IAITO_DEBUG

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_SCALE 1

using namespace iaito;
using namespace bonekeep;

int main(int argc, char* argv[])
{
	// Initialise log
	Log::Init(Debug, "debug_log.txt");

	// Initialise random
	time_t seed;
	time(&seed);
	srand(time(0));

	// Initialise game
	Game game;

	// Register services
	game.Register<EntityManager>();
	game.Register<SpriteRenderer>();
	game.Register<DebugRenderer>();
	game.Register<SoundManager>();
	game.Register<ViewManager>();
	game.Register<CollisionManager>();

	// Register components
	game.Get<EntityManager>().Register<Position>();
	game.Get<EntityManager>().Register<Physics>();
	game.Get<EntityManager>().Register<Sprite>();
	game.Get<EntityManager>().Register<Control>();
	game.Get<EntityManager>().Register<Collision>();

	// Register systems
	game.Register<FPS>();
	game.Register<ESC>();
	game.Register<Animate>();
	game.Register<Bounce>();
	game.Register<Render>();
	game.Register<Controller>();
	game.Register<PickupItem>();
	game.Register<CheckPlayerDeath>();
	game.Register<TestCollisions>();

	// Set display size
	game.GetContext().SetSize(SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE);
	game.Get<ViewManager>().SetSize(SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE);
	game.Get<ViewManager>().SetScale(SCREEN_SCALE);
	game.Get<SpriteRenderer>().SetAspectRatio(0.67f);
	game.Get<DebugRenderer>().SetAspectRatio(0.67f);

	// Spawn entities
	SpawnScenery(game);
	SpawnPlayer(game);
	SpawnPickup(game);
	SpawnEnemy(game, 40.0f, 40.0f);
	SpawnEnemy(game, 40.0f, 440.0f);
	SpawnEnemy(game, 600.0f, 40.0f);
	SpawnEnemy(game, 600.0f, 440.0f);

	// Bind audio
	game.Get<SoundManager>().BindAudio("music/fire");
	game.Get<SoundManager>().BindAudio("sfx/pickup");
	game.Get<SoundManager>().BindAudio("sfx/skeleton");
	game.Get<SoundManager>().BindAudio("sfx/death");
	game.Get<SoundManager>().Loop(0);

	// Let's go!
	game.Start();
	
	return 0;
}
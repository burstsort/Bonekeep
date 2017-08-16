#ifndef BONEKEEP_BASIC_COMPONENTS_HPP
#define BONEKEEP_BASIC_COMPONENTS_HPP

namespace bonekeep
{
	struct Position
	{
		float x;
		float y;
		float depth;
	};

	struct Physics
	{
		float hSpeed;
		float vSpeed;
	};

	struct Sprite
	{
		unsigned int spriteSheet;
		unsigned int spriteIndex;

		float hScale;
		float vScale;

		float angle;
		
		float frameIndex;
		float frameSpeed;

		char r;
		char g;
		char b;
	};

	struct Control
	{
		float speed;
		float score;
	};

	struct Collision
	{
		float xMin, yMin;
		float xMax, yMax;
	};
}

#endif // BONEKEEP_BASIC_COMPONENTS_HPP
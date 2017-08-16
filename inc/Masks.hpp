#ifndef BONEKEEP_MASKS_HPP
#define BONEKEEP_MASKS_HPP

namespace bonekeep
{
	enum Mask
	{
		NONE 		= 0,
		POSITION 	= 1 << 0,
		PHYSICS		= 1 << 1,
		SPRITE 		= 1 << 2, 
		CONTROL 	= 1 << 3,
		COLLISION	= 1 << 4
	};

	enum Tags
	{
		NOTAG 		= 0,
		PLAYER 		= 1 << 0,
		ENEMY 		= 1 << 1,
		PICKUP 		= 1 << 2,
		SCENERY 	= 1 << 3
	};
}

#endif // BONEKEEP_MASKS_HPP
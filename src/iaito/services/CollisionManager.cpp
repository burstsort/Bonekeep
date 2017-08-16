#include "iaito/services/CollisionManager.hpp"

#include <algorithm>

namespace iaito
{
	CollisionManager::CollisionManager() : _cellSize(64) {}
	CollisionManager::~CollisionManager(){}

	void CollisionManager::Insert(float xMin, float yMin, float xMax, float yMax, unsigned int id)
	{
		// Insert collider onto grid
		for (int y = yMin / _cellSize; y < yMax / _cellSize; ++y)
			for (int x = xMin / _cellSize; x < xMax / _cellSize; ++x)
				_grid[std::make_pair((int) x, (int) y)].push_back(id);
	}

	std::vector<unsigned int> CollisionManager::Query(float xMin, float yMin, float xMax, float yMax)
	{
		std::vector<unsigned int> result;

		// Query grid for possible colliders
		for (int y = yMin / _cellSize; y < yMax / _cellSize; ++y)
			for (int x = xMin / _cellSize; x < xMax / _cellSize; ++x)
				if (_grid.count(std::make_pair(x, y)) > 0)
				{
					std::vector<unsigned int>& nearby = _grid[std::make_pair(x, y)];
					result.insert(result.end(), nearby.begin(), nearby.end());
				}

		// Remove duplicates
		sort(result.begin(), result.end());
		result.erase(unique(result.begin(), result.end()), result.end());

		return result;
	}
	
	void CollisionManager::Clear()
	{
		_grid.clear();
	}

	bool CollisionManager::CheckCollision(float xMin1, float yMin1, float xMax1, float yMax1, float xMin2, float yMin2, float xMax2, float yMax2)
	{
		if (xMax1 < xMin2) return false;
		if (xMin1 > xMax2) return false;

		if (yMax1 < yMin2) return false;
		if (yMin1 > yMax2) return false;

		return true;
	}
}
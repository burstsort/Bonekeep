#ifndef IAITO_COLLISION_MANAGER_HPP
#define IAITO_COLLISION_MANAGER_HPP

#include <utility>
#include <map>
#include <vector>

#include "iaito/core/Service.hpp"
#include "iaito/resources/Collider.hpp"
#include "iaito/core/ResourceManager.hpp"

namespace iaito
{
	class CollisionManager: public Service
	{
		public:
			CollisionManager();
			~CollisionManager();

			void Insert(float xMin, float yMin, float xMax, float yMax, unsigned int id);
			std::vector<unsigned int> Query(float xMin, float yMin, float xMax, float yMax);
			void Clear();

			bool CheckCollision(float xMin1, float yMin1, float xMax1, float yMax1, float xMin2, float yMin2, float xMax2, float yMax2);

		private:
			CollisionManager(const CollisionManager& that);
			CollisionManager& operator=(const CollisionManager& that);

			ResourceManager<Collider> _colMgr;

			unsigned int _cellSize;
			std::map< std::pair<int, int>, std::vector<unsigned int> > _grid;
	};
}

#endif // IAITO_COLLISION_MANAGER_HPP
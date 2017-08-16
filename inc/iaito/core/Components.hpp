#ifndef IAITO_COMPONENTS_HPP
#define IAITO_COMPONENTS_HPP

#include <vector>

namespace iaito
{
	class ComponentsBase
	{
		friend class EntityManager;

		protected:
			virtual ~ComponentsBase(){};
			virtual void Add() = 0;
			virtual void Reset(unsigned int index) = 0;
			virtual void Clear() = 0;
	};

	template <typename T>
	class Components: public ComponentsBase
	{
		friend class EntityManager;
		
		public:
			T& Get(unsigned int index)
			{
				// Check if invalid index
				if(index >= _components.size())
					return _dummy;

				return _components[index];
			}

		protected:
			void Add()
			{
				_components.push_back(T());
			}

			void Reset(unsigned int index)
			{
				// Check if invalid index
				if (index >= _components.size())
					return;

				_components[index] = T();
			}

			void Clear()
			{
				_components.clear();
				_components.swap(_components);
			}
		
		private:
			// Components
			std::vector<T> _components;
			T _dummy;
	};
}

#endif // IAITO_COMPONENTS_HPP
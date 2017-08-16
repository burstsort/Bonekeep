#ifndef IAITO_RESOURCE_MANAGER_HPP
#define IAITO_RESOURCE_MANAGER_HPP

#include <string>
#include <vector>

#include "iaito/core/Resource.hpp"

namespace iaito
{
	template <typename T>
	class ResourceManager
	{
		public:
			unsigned int Bind(const std::string& filename)
			{
				// Check if invalid filename
				if (filename.empty())
					return -1;

				// Check if resource is already loaded
				for (unsigned int i = 0; i < _filenames.size(); ++i)
					if(_filenames[i] == filename)
					{
						_binds[i]++;
						return i;
					}

				// Create a new resource
				unsigned int index;

				// Check for empty index
				if (_empty.empty())
				{
					// Get index
					index = _resources.size();

					// Create a new resource
					_resources.push_back(T());
					_filenames.push_back(filename);
					_binds.push_back(1);
				}
				else
				{
					// Get index
					index = _empty.back();
					_empty.pop_back();

					// Reset resource
					_resources[index] = T();
					_filenames[index] = filename;
					_binds[index] = 1;
				}

				// Load
				_resources[index].Load(filename);

				return index;
			}

			void Unbind(unsigned int index)
			{
				// Check if invalid index
				if (index >= _resources.size())
					return;

				//Check if already marked as empty
				for (unsigned int i = 0; i < _empty.size(); ++i)
					if (_empty[i] == index)
						return;

				// Unbind
				if (_binds[index] != 0)
					_binds[index]--;

				// If no longer used destroy it
				if (_binds[index] == 0)
				{
					_empty.push_back(index);
					_resources[index].Unload();
					_resources[index] = T();
					_filenames[index].clear();
				}
			}

			void Reload()
			{
				// Reload all resources
				for (unsigned int i = 0; i < _resources.size(); ++i)
				{
					_resources[i].Unload();
					_resources[i].Load(_filenames[i]);
				}
			}

			void Clear()
			{
				// Unload resources
				for (unsigned int i = 0; i < _resources.size(); ++i)
					_resources[i].Unload();

				// Clear memory
				_resources.clear();
				_empty.clear();
				_filenames.clear();
				_binds.clear();

				// Reallocate memory
				_resources.swap(_resources);
				_empty.swap(_empty);
				_filenames.swap(_filenames);
				_binds.swap(_binds);
			}

			unsigned int Size()
			{
				// Return number of resources
				return _resources.size();
			}

			T& Get(unsigned int index)
			{
				// Check if invalid index
				if (index >= _resources.size())
					return _default;

				// Check if invalid resource
				if (_filenames[index].empty())
					return _default;
				
				return _resources[index];
			}

			std::string GetFilename(unsigned int index)
			{
				// Check if invalid index
				if (index >= _filenames.size())
					return "";

				return _filenames[index];
			}

		private:
			std::vector<T> _resources;
			T _default;

			std::vector<unsigned int> _empty;

			std::vector<std::string> _filenames;
			std::vector<unsigned int> _binds;
	};
}

#endif // IAITO_RESOURCE_MANAGER_HPP
#include "iaito/services/EntityManager.hpp"

namespace iaito
{
	EntityManager::EntityManager() : _nextId(1){}

	EntityManager::~EntityManager()
	{
		for (unsigned int i = 0; i < _types.size(); ++i)
			delete _map[_types[i]];
	}

	unsigned int EntityManager::Create()
	{
		unsigned int index;

		// Check for empty index
		if (_empty.empty())
		{
			// Get index
			index = _ids.size();

			// Create a new entity
			_ids.push_back(_nextId++);
			_masks.push_back(0);
			_tags.push_back(0);

			// Create new components
			for (unsigned int i = 0; i < _types.size(); ++i)
				_map[_types[i]]->Add();
		}
		else
		{
			// Get index
			index = _empty.back();
			_empty.pop_back();

			// Reset entity
			_ids[index] = _nextId++;
			_masks[index] = 0;
			_tags[index] = 0;

			// Reset components
			for (unsigned int i = 0; i < _types.size(); ++i)
				_map[_types[i]]->Reset(index);
		}

		return index;
	}

	void EntityManager::Destroy(unsigned int index)
	{
		// Check if invalid index
		if (index >= _ids.size())
			return;

		// Disable entity
		_ids[index] = 0;
		_masks[index] = 0;
		_tags[index] = 0;

		//Check if already marked as empty
		for (unsigned int i = 0; i < _empty.size(); ++i)
			if (_empty[i] == index)
				return;

		// Mark as empty
		_empty.push_back(index);
	}

	void EntityManager::Clear()
	{
		for (unsigned int i = 0; i < _types.size(); ++i)
			_map[_types[i]]->Clear();

		// Clear memory
		_empty.clear();
		_masks.clear();
		_tags.clear();
		_ids.clear();
		
		// Reallocate memory
		_empty.swap(_empty);
		_masks.swap(_masks);
		_tags.swap(_tags);
		_ids.swap(_ids);
	}

	unsigned int EntityManager::Size()
	{
		// Return number of entities
		return _ids.size();
	}

	unsigned long EntityManager::GetId(unsigned int index)
	{
		// Check if invalid index
		if (index >= _ids.size())
			return 0;
		
		return _ids[index];
	}

	unsigned int EntityManager::GetIndex(unsigned long id)
	{
		if (id == 0)
			return -1;

		// Find entity by id
		for (unsigned int i = 0; i < _ids.size(); ++i)
			if (_ids[i] == id)
				return i;

		return -1;
	}

	void EntityManager::Enable(unsigned int index, int mask)
	{
		// Check if invalid index
		if (index >= _ids.size())
			return;

		// Enable component bits
		_masks[index] |= mask;
	}

	void EntityManager::Disable(unsigned int index, int mask)
	{
		// Check if invalid index
		if (index >= _ids.size())
			return;

		// Disable component bits
		_masks[index] &= ~mask;
	}

	bool EntityManager::Check(unsigned int index, int mask)
	{
		// Check if invalid index
		if (index >= _ids.size())
			return false;

		// Check component bits
		return ((_masks[index] & mask) == mask);
	}

	void EntityManager::Tag(unsigned int index, int tag)
	{
		// Check if invalid index
		if (index >= _ids.size())
			return;

		// Enable tag bits
		if (_ids[index])
			_tags[index] |= tag;
	}

	void EntityManager::Untag(unsigned int index, int tag)
	{
		// Check if invalid index
		if (index >= _ids.size())
			return;

		// Disable tag bits
			_tags[index] &= ~tag;
	}

	bool EntityManager::CheckTag(unsigned int index, int tag)
	{
		// Check if invalid index
		if (index >= _ids.size())
			return false;

		// Check tag bits
		return ((_tags[index] & tag) == tag);
	}
}
#include "HashMap.h"

namespace FIEAGameEngine
{
#pragma region HASHMAP CONSTRUCTORS
	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(size_type capacity, HashFunct hashFunct, EqualityFunct equalityFunct) :
		_map{ (capacity > 1u) ? capacity : 17u }, _count{ 0u }, _hashFunct{ hashFunct }, _equalityFunct{ equalityFunct }
	{
		if (capacity <= 1u) capacity = 17u;
		for (size_type i = 0; i < capacity; ++i) _map.EmplaceBack();
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(std::initializer_list<_TPair> list)
	{
		for (size_type i = 0; i < _map.Capacity(); ++i) _map.EmplaceBack();

		for (auto& value : list)
		{
			Insert(std::move(value));
		}
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(HashMap&& other) noexcept :
		_map{ std::move(other._map) }, _count{ other._count }, _hashFunct{ other._hashFunct }, _equalityFunct{ other._equalityFunct }
	{
		other._count = 0;
		//for (size_type i = 0; i < _map.Capacity(); ++i) other._map.EmplaceBack();
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(HashMap&& other) noexcept
	{
		if (this != &other)
		{
			_map = std::move(other._map);
			_count = other._count;
			_hashFunct = other._hashFunct;
			_equalityFunct = other._equalityFunct;

			other._count = 0;
			//for (size_type i = 0; i < _map.Capacity(); ++i) other._map.EmplaceBack();
		}

		return *this;
	}
#pragma endregion

#pragma region ITERATOR CONSTRUCTORS
	template <typename TKey, typename TData>
	HashMap<TKey, TData>::Iterator::Iterator(HashMap& owner, size_type listNum, typename _TList::Iterator itrElement) :
		_owner{ &owner }, _listNum{ listNum }, _itrElement{ itrElement }
	{
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::Const_Iterator::Const_Iterator(const HashMap& owner, size_type listNum, typename _TList::Const_Iterator itrElement) :
		_owner{ &owner }, _listNum{ listNum }, _itrElement{ itrElement }
	{
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::Const_Iterator::Const_Iterator(const Iterator& other) :
		_owner{ other._owner }, _listNum{ other._listNum }, _itrElement{ other._itrElement }
	{
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Const_Iterator& HashMap<TKey, TData>::Const_Iterator::operator=(const Iterator& other)
	{
		_owner = other._owner;
		_listNum = other._listNum;
		_itrElement = other._itrElement;

		return *this;
	}
#pragma endregion

#pragma region ITERATOR FUNCTIONS
	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::Iterator::operator==(const Iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::Iterator::operator!=(const Iterator& other) const
	{
		return ((_owner != other._owner) || (_listNum != other._listNum) || ((_listNum < _owner->_map.Size()) && (_itrElement != other._itrElement)));
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator++()
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");

		if (_listNum < _owner->_map.Size() && ++_itrElement == _owner->_map[_listNum].end())
		{
			for (++_listNum; _listNum < _owner->_map.Size(); ++_listNum)
			{
				if (!_owner->_map[_listNum].IsEmpty())
				{
					_itrElement = _owner->_map[_listNum].begin();
					break;
				}
			}
		}

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator prev = *this;
		operator++();
		return prev;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator::reference HashMap<TKey, TData>::Iterator::operator*() const
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");
		if (_listNum >= _owner->_map.Size()) throw std::runtime_error("Iterator points to end().");

		return *_itrElement;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator::pointer HashMap<TKey, TData>::Iterator::operator->() const
	{
		return &operator*();
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::Const_Iterator::operator==(const Const_Iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::Const_Iterator::operator!=(const Const_Iterator& other) const
	{
		return ((_owner != other._owner) || (_listNum != other._listNum) || ((_listNum < _owner->_map.Size()) && (_itrElement != other._itrElement)));
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Const_Iterator& HashMap<TKey, TData>::Const_Iterator::operator++()
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");

		if (_listNum < _owner->_map.Size() && ++_itrElement == _owner->_map[_listNum].end())
		{
			for (++_listNum; _listNum < _owner->_map.Size(); ++_listNum)
			{
				if (!_owner->_map[_listNum].IsEmpty())
				{
					_itrElement = _owner->_map[_listNum].cbegin();
					break;
				}
			}

		}

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Const_Iterator HashMap<TKey, TData>::Const_Iterator::operator++(int)
	{
		Const_Iterator prev = *this;
		operator++();
		return prev;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Const_Iterator::const_reference HashMap<TKey, TData>::Const_Iterator::operator*() const
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");
		if (_listNum >= _owner->_map.Size()) throw std::runtime_error("Iterator points to end().");

		return *_itrElement;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Const_Iterator::const_pointer HashMap<TKey, TData>::Const_Iterator::operator->() const
	{
		return &operator*();
	}
#pragma endregion

#pragma region HASHMAP FUNCTIONS
	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::begin()
	{
		if (_count > 0u)
		{
			for (size_type i = 0; i < _map.Size(); ++i)
			{
				if (!_map[i].IsEmpty()) return Iterator{ *this, i, _map[i].begin() };
			}
		}

		return end();
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Const_Iterator HashMap<TKey, TData>::begin() const
	{
		if (_count > 0u)
		{
			for (size_type i = 0; i < _map.Size(); ++i)
			{
				if (!_map[i].IsEmpty()) return Const_Iterator{ *this, i, _map[i].cbegin() };
			}
		}

		return end();
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Const_Iterator HashMap<TKey, TData>::cbegin() const
	{
		if (_count > 0u)
		{
			for (size_type i = 0; i < _map.Size(); ++i)
			{
				if (!_map[i].IsEmpty()) return Const_Iterator{ *this, i, _map[i].cbegin() };
			}
		}

		return end();
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::end()
	{
		size_type mapsize = _map.Size();
		return Iterator{ *this, mapsize, _map[mapsize - 1].end() };
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Const_Iterator HashMap<TKey, TData>::end() const
	{
		size_type mapsize = _map.Size();
		return Const_Iterator{ *this, mapsize, _map[mapsize - 1].cend() };
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Const_Iterator HashMap<TKey, TData>::cend() const
	{
		size_type mapsize = _map.Size();
		return Const_Iterator{ *this, mapsize, _map[mapsize - 1].cend() };
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::size_type HashMap<TKey, TData>::Size() const
	{
		return _count;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::size_type HashMap<TKey, TData>::Capacity() const
	{
		return _map.Size();;
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::IsEmpty() const
	{
		return (_count == 0);
	}

	template <typename TKey, typename TData>
	float HashMap<TKey, TData>::LoadFactor(size_type threshold)
	{
		size_type filledcount = 0;
		for (const _TList& list : _map)
		{
			if (list.Size() > threshold) ++filledcount;
		}

		return (static_cast<float>(filledcount) / _map.Size());
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::HashMap::Iterator HashMap<TKey, TData>::HashMap::Find(const TKey& key)
	{
		size_type index = _hashFunct(key) % _map.Size();
		_TList& list = _map[index];

		for (typename _TList::Iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			if (_equalityFunct((*itr).first, key))
			{
				return Iterator{ *this,index, itr };
			}
		}

		return end();
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::HashMap::Const_Iterator HashMap<TKey, TData>::HashMap::Find(const TKey& key) const
	{
		size_type index = _hashFunct(key) % _map.Size();
		const _TList& list = _map[index];

		for (typename _TList::Const_Iterator itr = list.cbegin(); itr != list.cend(); ++itr)
		{
			if (_equalityFunct((*itr).first, key))
			{
				return Const_Iterator{ *this,index, itr };
			}
		}

		return cend();
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::HashMap::ContainsKey(const TKey& key) const
	{
		const _TList& list = _map[_hashFunct(key) % _map.Size()];

		bool success = false;
		for (typename _TList::Const_Iterator itr = list.cbegin(); itr != list.cend(); ++itr)
		{
			if (_equalityFunct((*itr).first, key))
			{
				success = true;
			}
		}

		return success;
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::HashMap::ContainsKey(const TKey& key, TData*& data)
	{
		_TList& list = _map[_hashFunct(key) % _map.Size()];

		bool success = false;
		for (typename _TList::Iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			if (_equalityFunct((*itr).first, key))
			{
				data = &((*itr).second);
				success = true;
			}
		}

		return success;
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::HashMap::ContainsKey(const TKey& key, const TData*& data) const
	{
		const _TList& list = _map[_hashFunct(key) % _map.Size()];

		bool success = false;
		for (typename _TList::Const_Iterator itr = list.cbegin(); itr != list.cend(); ++itr)
		{
			if (_equalityFunct((*itr).first, key))
			{
				data = &((*itr).second);
				success = true;
			}
		}

		return success;
	}

	template <typename TKey, typename TData>
	std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const _TPair& datapair)
	{
		size_type index = _hashFunct(datapair.first) % _map.Size();
		_TList& list = _map[index];

		for (typename _TList::Iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			if (_equalityFunct((*itr).first, datapair.first))
			{
				return std::pair<Iterator, bool>(Iterator{ *this, index, itr }, false);
			}
		}

		++_count;
		return std::pair <Iterator, bool>(Iterator{ *this, index, list.PushBack(datapair) }, true);
	}

	template <typename TKey, typename TData>
	std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const TKey& key, const TData& data)
	{
		return Insert(std::pair<const TKey, TData>(key, data));
	}

	template <typename TKey, typename TData>
	std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(_TPair&& datapair)
	{
		size_type index = _hashFunct(datapair.first) % _map.Size();
		_TList& list = _map[index];

		for (typename _TList::Iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			if (_equalityFunct((*itr).first, datapair.first))
			{
				return std::pair<Iterator, bool>(Iterator{ *this, index, itr }, false);
			}
		}

		++_count;
		return std::pair <Iterator, bool>(Iterator{ *this, index, list.PushBack(std::move(datapair)) }, true);
	}

	template <typename TKey, typename TData>
	TData& HashMap<TKey, TData>::At(const TKey& key)
	{
		size_type index = _hashFunct(key) % _map.Size();
		_TList& list = _map[index];

		for (typename _TList::Iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			if (_equalityFunct((*itr).first, key))
			{
				return (*itr).second;
			}
		}

		throw std::out_of_range("Key out of range");
	}

	template <typename TKey, typename TData>
	const TData& HashMap<TKey, TData>::At(const TKey& key) const
	{
		size_type index = _hashFunct(key) % _map.Size();
		const _TList& list = _map[index];

		for (typename _TList::Const_Iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			if (_equalityFunct((*itr).first, key))
			{
				return (*itr).second;
			}
		}

		throw std::out_of_range("Key out of range");
	}

	template <typename TKey, typename TData>
	TData& HashMap<TKey, TData>::operator[](const TKey& key)
	{
		size_type index = _hashFunct(key) % _map.Size();
		_TList& list = _map[index];

		for (typename _TList::Iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			if (_equalityFunct((*itr).first, key))
			{
				return (*itr).second;
			}
		}

		++_count;
		return (*(list.PushBack(_TPair(key, TData())))).second;
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::Remove(const TKey& key)
	{
		size_type index = _hashFunct(key) % _map.Size();
		_TList& list = _map[index];

		bool success = false;
		for (typename _TList::Iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			if (_equalityFunct((*itr).first, key))
			{
				success = list.Remove(itr);
				break;
			}
		}

		if (success) --_count;
		return success;
	}

	template <typename TKey, typename TData>
	void HashMap<TKey, TData>::Clear()
	{
		for (_TList& list : _map)
		{
			list.Clear();
		}

		_count = 0;
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::ReHash(size_type capacity)
	{
		if (capacity > 1u)
		{
			_TMap newMap{ capacity };
			for (size_type i = 0; i < capacity; ++i) newMap.EmplaceBack();

			for (const _TList& _list : _map)
			{
				for (const _TPair& element : _list)
				{
					newMap[_hashFunct(element.first) % capacity].PushBack(element);
				}
			}

			_map = std::move(newMap);
			return true;
		}

		return false;
	}

	template <typename TKey, typename TData>
	void HashMap<TKey, TData>::ReHash(HashFunct newHashFunct)
	{
		size_type capacity = _map.Capacity();
		_TMap newMap{ capacity };
		for (size_type i = 0; i < capacity; ++i) newMap.EmplaceBack();

		for (const _TList& _list : _map)
		{
			for (const _TPair& element : _list)
			{
				newMap[newHashFunct(element.first) % capacity].PushBack(element);
			}
		}

		_map = std::move(newMap);
		_hashFunct = newHashFunct;
	}

	template <typename TKey, typename TData>
	void HashMap<TKey, TData>::ReHash(EqualityFunct newEqualityFunct)
	{
		size_type capacity = _map.Capacity();
		_TMap newMap{ capacity };
		for (size_type i = 0; i < capacity; ++i) newMap.EmplaceBack();

		_count = 0;
		for (const _TList& _list : _map)
		{
			for (const _TPair& element : _list)
			{
				bool found = false;

				size_type index = _hashFunct(element.first) % capacity;
				_TList& list = newMap[index];

				for (typename _TList::Iterator itr = list.begin(); itr != list.end(); ++itr)
				{
					if (newEqualityFunct((*itr).first, element.first))
					{
						found = true;
					}
				}

				if (!found)
				{
					list.PushBack(element);
					++_count;
				}
			}
		}
		_map = std::move(newMap);
		_equalityFunct = newEqualityFunct;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::HashFunct HashMap<TKey, TData>::GetHashFunct() const
	{
		return _hashFunct;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::EqualityFunct HashMap<TKey, TData>::GetEqualityFunct() const
	{
		return _equalityFunct;
	}
#pragma endregion

}
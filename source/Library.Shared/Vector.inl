#include "Vector.h"

namespace FIEAGameEngine
{

#pragma region VECTOR CONSTRUCTORS
	template <typename T>
	Vector<T>::Vector(size_type capacity) :
		_baseData{ nullptr },
		_size{ 0u }, _capacity{ 0u }
	{
		Reserve(capacity);
	}

	template <typename T>
	Vector<T>::Vector(std::initializer_list<T> list) :
		_baseData{ nullptr },
		_size{ 0u }, _capacity{ 0u }
	{
		Reserve(list.size());

		for (auto& value : list)
		{
			PushBack(value);
		}
	}

	template <typename T>
	Vector<T>::Vector(const Vector& other) :
		_baseData{ nullptr },
		_size{ other._size }, _capacity{ 0u }
	{
		if (other._size > 0u)
		{
			Reserve(other._capacity);

			for (size_type i = 0; i < other._size; ++i)
			{
				new(_baseData + i) value_type{ *(other._baseData + i) };
			}
		}
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			if (_size > other._size)
			{
				for (size_type i = 0; i < _size - other._size; ++i) PopBack();
			}

			size_type maxSize = (_size > other._size) ? _size : other._size;
			for (size_type i = 0; i < maxSize; ++i)
			{
				if (i < _size) *(_baseData + i) = *(other._baseData + i);
				else PushBack(other[i]);
			}
		}

		return *this;
	}

	template <typename T>
	Vector<T>::Vector(Vector&& other) noexcept :
		_baseData{ other._baseData },
		_size{ other._size }, _capacity{ other._capacity }
	{
		other._baseData = nullptr;
		other._size = other._capacity = 0u;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			free(_baseData);

			_baseData = other._baseData;
			_size = other._size;
			_capacity = other._capacity;

			other._baseData = nullptr;
			other._size = other._capacity = 0u;
		}

		return *this;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator+(Vector&& other)
	{
		if (this != &other && !other.IsEmpty())
		{
			Reserve(_size + other._size);

			for (T& value : other)
			{
				PushBack(std::move(value));
			}

			other._baseData = nullptr;
			other._size = other._capacity = 0u;
		}

		return *this;
	}
#pragma endregion

#pragma region ITERATOR CONSTRUCTORS
	template <typename T>
	Vector<T>::Iterator::Iterator(const Vector& owner, size_type index) :
		_owner{ &owner }, _curIndex{ index }
	{
	}

	template <typename T>
	Vector<T>::Iterator::Iterator(const Iterator& other) :
		_owner{ other._owner }, _curIndex{ other._curIndex }
	{
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(const Iterator& other)
	{
		_owner = other._owner;
		_curIndex = other._curIndex;
		return *this;
	}

	template <typename T>
	Vector<T>::Const_Iterator::Const_Iterator(const Vector& owner, size_type index) :
		_owner{ &owner }, _curIndex{ index }
	{
	}

	template <typename T>
	Vector<T>::Const_Iterator::Const_Iterator(const Const_Iterator& other) :
		_owner{ other._owner }, _curIndex{ other._curIndex }
	{
	}

	template <typename T>
	Vector<T>::Const_Iterator::Const_Iterator(const Iterator& other) :
		_owner{ other._owner }, _curIndex{ other._curIndex }
	{
	}

	template <typename T>
	typename Vector<T>::Const_Iterator& Vector<T>::Const_Iterator::operator=(const Iterator& other)
	{
		_owner = other._owner;
		_curIndex = other._curIndex;
		return *this;
	}

	template <typename T>
	typename Vector<T>::Const_Iterator& Vector<T>::Const_Iterator::operator=(const Const_Iterator& other)
	{
		_owner = other._owner;
		_curIndex = other._curIndex;
		return *this;
	}
#pragma endregion

#pragma region ITERATOR FUNCTIONS
	template <typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator& other) const
	{
		return ((_owner != other._owner) || (_curIndex != other._curIndex));
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");

		if (_curIndex < _owner->_size) ++_curIndex;

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator prev = *this;
		operator++();
		return prev;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");

		if (_curIndex > 0) --_curIndex;

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator prev = *this;
		operator--();
		return prev;
	}

	template <typename T>
	T& Vector<T>::Iterator::operator*() const
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");
		if (_curIndex == _owner->_size) throw std::runtime_error("Iterator points to end.");

		return *(_owner->_baseData + _curIndex);
	}

	template <typename T>
	bool Vector<T>::Const_Iterator::operator==(const Const_Iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename T>
	bool Vector<T>::Const_Iterator::operator!=(const Const_Iterator& other) const
	{
		return ((_owner != other._owner) || (_curIndex != other._curIndex));
	}

	template <typename T>
	typename Vector<T>::Const_Iterator& Vector<T>::Const_Iterator::operator++()
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");

		if (_curIndex < _owner->_size) ++_curIndex;

		return *this;
	}

	template <typename T>
	typename Vector<T>::Const_Iterator Vector<T>::Const_Iterator::operator++(int)
	{
		Const_Iterator prev = *this;
		operator++();
		return prev;
	}

	template <typename T>
	typename Vector<T>::Const_Iterator& Vector<T>::Const_Iterator::operator--()
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");

		if (_curIndex > 0) --_curIndex;

		return *this;
	}

	template <typename T>
	typename Vector<T>::Const_Iterator Vector<T>::Const_Iterator::operator--(int)
	{
		Const_Iterator prev = *this;
		operator--();
		return prev;
	}

	template <typename T>
	const T& Vector<T>::Const_Iterator::operator*() const
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");
		if (_curIndex == _owner->_size) throw std::runtime_error("Iterator points to end.");

		return *(_owner->_baseData + _curIndex);
	}
#pragma endregion

#pragma region VECTOR FUNCTIONS

	template <typename T>
	bool Vector<T>::operator==(const Vector& rhs) const
	{
		if (this == &rhs) return true;
		if (_size != rhs._size) return false;

		for (size_type i = 0; i < _size; ++i)
		{
			if (_baseData[i] != rhs._baseData[i]) return false;
		}

		return true;
	}

	template <typename T>
	bool Vector<T>::operator!=(const Vector& rhs) const
	{
		return !operator==(rhs);
	}

	template <typename T>
	bool Vector<T>::ComparePointerData(const Vector& rhs) const
	{
		if (this == &rhs) return true;
		if (_size != rhs._size) return false;

		for (size_type i = 0; i < _size; ++i)
		{
			if (*_baseData[i] != *rhs._baseData[i]) return false;
		}

		return true;
	}

	template <typename T>
	T& Vector<T>::operator[](size_type index)
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range.");

		return *(_baseData + index);
	}

	template <typename T>
	const T& Vector<T>::operator[](size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range.");

		return *(_baseData + index);
	}

	template <typename T>
	T& Vector<T>::At(size_type index)
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range.");

		return *(_baseData + index);
	}

	template <typename T>
	const T& Vector<T>::At(size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range.");

		return *(_baseData + index);
	}

	template <typename T>
	bool Vector<T>::Reserve(size_type capacity)
	{
		if (capacity > _capacity)
		{
			void* alloc = std::realloc(_baseData, sizeof(value_type) * capacity);
			if (alloc)
			{
				_baseData = reinterpret_cast<pointer>(alloc);
				_capacity = capacity;
				return true;
			}
			else
			{
				throw std::runtime_error("Failed to Allocate Memory");
			}
		}

		return false;
	}

	template <typename T>
	bool Vector<T>::ShrinkToFit(size_type capacity)
	{
		if (capacity == 0 && _size == 0)
		{
			free(_baseData);
			_baseData = nullptr;
			_capacity = 0;
		}
		else if (capacity >= _size && capacity < _capacity)
		{
			void* alloc = std::realloc(_baseData, sizeof(value_type) * capacity);
			if (alloc)
			{
				_baseData = reinterpret_cast<pointer>(alloc);
				_capacity = capacity;
				return true;
			}
		}


		return false;
	}

	template <typename T>
	bool Vector<T>::ShrinkToFit()
	{
		return ShrinkToFit(_size);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::PushBack(const_reference data)
	{
		if (((_size >= _capacity) && (Reserve(std::max<size_type>(_incrementFunctor(_size, _capacity), _capacity + 1))))
			|| (_size < _capacity))
		{
			new(_baseData + _size) value_type{ data };

			return Iterator{ *this, _size++ };
		}

		return Iterator{ *this, _size };
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::PushBack(value_type&& data)
	{
		return EmplaceBack(std::forward<T>(data));
	}

	template <typename T>
	template <typename... Args>
	typename Vector<T>::Iterator Vector<T>::EmplaceBack(Args&&... args)
	{
		if (((_size >= _capacity) && (Reserve(std::max<size_type>(_incrementFunctor(_size, _capacity), _capacity + 1))))
			|| (_size < _capacity))
		{
			new(_baseData + _size) value_type{ std::forward<Args>(args)... };

			return Iterator{ *this, _size++ };
		}

		return Iterator{ *this, _size };
	}

	template <typename T>
	void Vector<T>::PopBack()
	{
		if (_size > 0u)
		{
			--_size;
			(_baseData + _size)->~T();
		}
	}

	template <typename T>
	bool Vector<T>::IsEmpty() const { return (_size < 1u); }

	template <typename T>
	std::size_t Vector<T>::Size() const { return _size; }

	template <typename T>
	std::size_t Vector<T>::Capacity() const { return _capacity; }

	template <typename T>
	T& Vector<T>::Front()
	{
		if (_size < 1u) throw std::runtime_error("Cannot access Front() from Empty Vector.");

		return *_baseData;
	}

	template <typename T>
	const T& Vector<T>::Front() const
	{
		if (_size < 1u) throw std::runtime_error("Cannot access Front() from Empty Vector.");

		return *_baseData;
	}

	template <typename T>
	T& Vector<T>::Back()
	{
		if (_size < 1u) throw std::runtime_error("Cannot access Back() from Empty Vector.");

		return *(_baseData + _size - 1);
	}

	template <typename T>
	const T& Vector<T>::Back() const
	{
		if (_size < 1u) throw std::runtime_error("Cannot access Back() from Empty Vector.");

		return *(_baseData + _size - 1);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator{ *this, 0 };
	}

	template <typename T>
	typename Vector<T>::Const_Iterator Vector<T>::begin() const
	{
		return Const_Iterator{ *this, 0 };
	}

	template <typename T>
	typename Vector<T>::Const_Iterator Vector<T>::cbegin() const
	{
		return Const_Iterator{ *this, 0 };
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator{ *this, _size };
	}

	template <typename T>
	typename Vector<T>::Const_Iterator Vector<T>::end() const
	{
		return Const_Iterator{ *this, _size };
	}

	template <typename T>
	typename Vector<T>::Const_Iterator Vector<T>::cend() const
	{
		return Const_Iterator{ *this, _size };
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		for (size_type i = 0; i < _size; ++i)
		{
			(_baseData + i)->~T();
		}
		_size = 0;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const_reference value, std::function<bool(const_reference, const_reference)> equalityCheck)
	{
		Iterator it = begin();

		for (; it != end(); ++it)
		{
			if (equalityCheck(*it, value)) break;
		}

		return it;
	}

	template <typename T>
	typename Vector<T>::Const_Iterator Vector<T>::Find(const_reference value, std::function<bool(const_reference, const_reference)> equalityCheck) const
	{
		Const_Iterator it = begin();

		for (; it != end(); ++it)
		{
			if (equalityCheck(*it, value)) break;
		}

		return it;
	}

	template <typename T>
	bool Vector<T>::Remove(const_reference value, Iterator start, Iterator finish, std::function<bool(const_reference, const_reference)> equalityCheck)
	{
		if (start._owner != this) throw std::runtime_error("Start Iterator doesn't belong to this Vector");
		if (finish._owner != this) throw std::runtime_error("Finish Iterator doesn't belong to this Vector");

		if (start._curIndex >= _size) return false;
		if (finish._curIndex >= _size || finish._curIndex < start._curIndex) finish._curIndex = _size;
		if (finish._curIndex == start._curIndex) ++finish._curIndex;

		for (; start != finish; ++start)
		{
			if (equalityCheck(*start, value))
			{
				(_baseData + start._curIndex)->~T();

				std::memmove(_baseData + start._curIndex, _baseData + (start._curIndex + 1), (_size - start._curIndex - 1) * sizeof(value_type));

				--_size;

				return true;
			}
		}

		return false;
	}

	template <typename T>
	bool Vector<T>::Remove(const_reference value, std::function<bool(const_reference, const_reference)> equalityCheck)
	{
		return Remove(value, begin(), end(), equalityCheck);
	}

	template <typename T>
	bool Vector<T>::Remove(Iterator itr)
	{
		if (itr._owner != this) throw std::runtime_error("Iterator doesn't belong to this Vector");

		if (itr._curIndex >= _size) return false;

		(_baseData + itr._curIndex)->~T();
		std::memmove(_baseData + itr._curIndex, _baseData + (itr._curIndex + 1), (_size - itr._curIndex - 1) * sizeof(T));
		--_size;

		return true;
	}

	template <typename T>
	bool Vector<T>::RemoveAll(const_reference value, Iterator start, Iterator finish, std::function<bool(const_reference, const_reference)> equalityCheck)
	{
		if (start._owner != this) throw std::runtime_error("Start Iterator doesn't belong to this Vector");
		if (finish._owner != this) throw std::runtime_error("Finish Iterator doesn't belong to this Vector");

		if (start._curIndex >= _size) return false;
		if (finish._curIndex >= _size || finish._curIndex < start._curIndex) finish._curIndex = _size;
		if (finish._curIndex == start._curIndex) ++finish._curIndex;

		bool retval = false;
		for (; start._curIndex < finish._curIndex; ++start)
		{
			if (equalityCheck(*start, value))
			{
				(_baseData + start._curIndex)->~T();

				Iterator next{ *this, start._curIndex + 1 };
				while (next._curIndex < _size && equalityCheck(*next, value)) { (_baseData + next._curIndex)->~T(); ++next; }

				size_type removeCount = next._curIndex - start._curIndex;
				std::memmove(_baseData + start._curIndex, _baseData + next._curIndex, sizeof(value_type) * (_size - next._curIndex));

				finish._curIndex -= removeCount;
				_size -= removeCount;
				retval = true;
			}
		}

		return retval;
	}

	template <typename T>
	bool Vector<T>::RemoveAll(const_reference value, std::function<bool(const_reference, const_reference)> equalityCheck)
	{
		return RemoveAll(value, begin(), end(), equalityCheck);
	}

	template <typename T>
	bool Vector<T>::RemoveAll(Iterator start, Iterator finish)
	{
		if (start._owner != this) throw std::runtime_error("Start Iterator doesn't belong to this Vector");
		if (finish._owner != this) throw std::runtime_error("Finish Iterator doesn't belong to this Vector");

		if (start._curIndex >= _size) return false;
		if (finish._curIndex >= _size || finish._curIndex < start._curIndex) finish._curIndex = _size;
		if (finish._curIndex == start._curIndex) ++finish._curIndex;

		Iterator destruct{ start };
		while (destruct != finish) { (_baseData + destruct._curIndex)->~T(); ++destruct; }

		size_type removeCount = finish._curIndex - start._curIndex;
		std::memmove(_baseData + start._curIndex, _baseData + finish._curIndex, sizeof(value_type) * (_size - finish._curIndex));

		finish._curIndex -= removeCount;
		_size -= removeCount;

		return true;
	}

	template <typename T>
	void  Vector<T>::SetIncrementFunctor(std::function<size_type(size_type, size_type)> incrementFunctor)
	{
		_incrementFunctor = incrementFunctor;
	}
#pragma endregion

	template <typename T>
	Vector<T>::~Vector()
	{
		Clear();
		free(_baseData);
	}

}
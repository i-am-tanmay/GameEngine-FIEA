#include "SList.h"

namespace FIEAGameEngine
{

#pragma region SLIST CONSTRUCTORS
	template <typename T>
	SList<T>::SList() :
		_size{ 0u },
		_front{ nullptr }, _back{ nullptr }
	{
	}

	template <typename T>
	SList<T>::SList(const SList& other) :
		_size{ 0u },
		_front{ nullptr }, _back{ nullptr }
	{
		if (other._size > 0u)
		{
			for (Node* othernode{ other._front }; othernode != nullptr; othernode = othernode->Next)
			{
				PushBack(othernode->Data);
			}
		}
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			if (_size > other._size)
			{
				for (std::size_t i = 0; i < _size - other._size; ++i) PopFront();
			}

			Node* this_front{ _front };
			Node* other_front{ other._front };

			while (this_front != nullptr)
			{
				this_front->Data = other_front->Data;
				this_front = this_front->Next;
				other_front = other_front->Next;
			}

			while (other_front != nullptr)
			{
				PushBack(other_front->Data);
				other_front = other_front->Next;
			}

		}

		return *this;
	}
#pragma endregion

#pragma region ITERATOR CONSTRUCTORS
	template <typename T>
	SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		_owner{ &owner }, _node{ node }
	{
	}

	template <typename T>
	SList<T>::Iterator::Iterator(const Iterator& other) :
		_owner{ other._owner }, _node{ other._node }
	{
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(const Iterator& other)
	{
		_owner = other._owner;
		_node = other._node;
		return *this;
	}

	template <typename T>
	SList<T>::Const_Iterator::Const_Iterator(const SList& owner, Node* node) :
		_owner{ &owner }, _node{ node }
	{
	}

	template <typename T>
	SList<T>::Const_Iterator::Const_Iterator(const Const_Iterator& other) :
		_owner{ other._owner }, _node{ other._node }
	{
	}

	template <typename T>
	SList<T>::Const_Iterator::Const_Iterator(const Iterator& other) :
		_owner{ other._owner }, _node{ other._node }
	{
	}

	template <typename T>
	typename SList<T>::Const_Iterator& SList<T>::Const_Iterator::operator=(const Iterator& other)
	{
		_owner = other._owner;
		_node = other._node;
		return *this;
	}

	template <typename T>
	typename SList<T>::Const_Iterator& SList<T>::Const_Iterator::operator=(const Const_Iterator& other)
	{
		_owner = other._owner;
		_node = other._node;
		return *this;
	}
#pragma endregion

#pragma region ITERATOR FUNCTIONS
	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename T>
	bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return ((_owner != other._owner) || (_node != other._node));
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");

		if (_node != nullptr) _node = _node->Next;

		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator prev = *this;
		operator++();
		return prev;
	}

	template <typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");
		if (_node == nullptr) throw std::runtime_error("Iterator points to nullptr.");

		return _node->Data;
	}

	template <typename T>
	bool SList<T>::Const_Iterator::operator==(const Const_Iterator& other) const
	{
		return ((_owner == other._owner) && (_node == other._node));
	}

	template <typename T>
	bool SList<T>::Const_Iterator::operator!=(const Const_Iterator& other) const
	{
		return !operator==(other);
	}

	template <typename T>
	typename SList<T>::Const_Iterator& SList<T>::Const_Iterator::operator++()
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");

		if (_node != nullptr) _node = _node->Next;

		return *this;
	}

	template <typename T>
	typename SList<T>::Const_Iterator SList<T>::Const_Iterator::operator++(int)
	{
		Const_Iterator prev = *this;
		operator++();
		return prev;
	}

	template <typename T>
	const T& SList<T>::Const_Iterator::operator*() const
	{
		if (_owner == nullptr) throw std::runtime_error("Iterator has no owner.");
		if (_node == nullptr) throw std::runtime_error("Iterator points to nullptr.");

		return _node->Data;
	}
#pragma endregion

#pragma region SLIST FUNCTIONS
	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& data)
	{
		_front = new Node(_front, data);
		++_size;
		if (_size == 1) _back = _front;

		return Iterator{ *this, _front };
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(T&& data)
	{
		_front = new Node(_front, std::move(data));
		++_size;
		if (_size == 1) _back = _front;

		return Iterator{ *this, _front };
	}

	template <typename T>
	template<typename... Args>
	typename SList<T>::Iterator SList<T>::EmplaceFront(Args&&... args)
	{
		_front = new Node(_front, std::forward<Args>(args)...);
		++_size;
		if (_size == 1) _back = _front;

		return Iterator{ *this, _front };
	}

	template <typename T>
	void SList<T>::PopFront()
	{
		if (_size > 0u)
		{
			Node* toDelete{ _front };
			_front = _front->Next;
			delete toDelete;

			--_size;
			if (_size < 1u) _front = _back = nullptr;
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& data)
	{
		if (_size < 1u) _front = _back = new Node(nullptr, data);
		else _back = _back->Next = new Node(nullptr, data);
		++_size;

		return Iterator{ *this, _back };
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(T&& data)
	{
		if (_size < 1u) _front = _back = new Node(nullptr, std::move(data));
		else _back = _back->Next = new Node(nullptr, std::move(data));
		++_size;

		return Iterator{ *this, _back };
	}

	template <typename T>
	template<typename... Args>
	typename SList<T>::Iterator SList<T>::EmplaceBack(Args&&... args)
	{
		if (_size < 1u) _front = _back = new Node(nullptr, std::forward<Args>(args)...);
		else _back = _back->Next = new Node(nullptr, std::forward<Args>(args)...);
		++_size;

		return Iterator{ *this, _back };
	}

	template <typename T>
	void SList<T>::PopBack()
	{
		if (_size > 0u)
		{
			Node* secondlastnode{ _front };
			while (secondlastnode->Next != nullptr)
			{
				if ((secondlastnode->Next)->Next == nullptr) break;
				secondlastnode = secondlastnode->Next;
			}
			delete _back;
			_back = secondlastnode;
			_back->Next = nullptr;

			--_size;
			if (_size < 1u) _front = _back = nullptr;
		}
	}

	template <typename T>
	inline bool SList<T>::IsEmpty() const { return (_size < 1u); }

	template <typename T>
	inline const std::size_t SList<T>::Size() const { return _size; }

	template <typename T>
	T& SList<T>::Front()
	{
		if (_size < 1u) throw std::runtime_error("Cannot access Front() from Empty List.");

		return _front->Data;
	}

	template <typename T>
	const T& SList<T>::Front() const
	{
		if (_size < 1u) throw std::runtime_error("Cannot access Front() from Empty List.");

		return _front->Data;
	}

	template <typename T>
	T& SList<T>::Back()
	{
		if (_size < 1u) throw std::runtime_error("Cannot access Back() from Empty List.");

		return _back->Data;
	}

	template <typename T>
	const T& SList<T>::Back() const
	{
		if (_size < 1u) throw std::runtime_error("Cannot access Back() from Empty List.");

		return _back->Data;
	}

	template <typename T>
	void SList<T>::Clear()
	{
		Node* toDelete{ _front };
		while (toDelete != nullptr)
		{
			_front = _front->Next;
			delete toDelete;
			toDelete = _front;
		}
		_back = nullptr;
		_size = 0u;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator{ *this, _front };
	}

	template <typename T>
	typename SList<T>::Const_Iterator SList<T>::begin() const
	{
		return Const_Iterator{ *this, _front };
	}

	template <typename T>
	typename SList<T>::Const_Iterator SList<T>::cbegin() const
	{
		return Const_Iterator{ *this, _front };
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator{ *this };
	}

	template <typename T>
	typename SList<T>::Const_Iterator SList<T>::end() const
	{
		return Const_Iterator{ *this };
	}

	template <typename T>
	typename SList<T>::Const_Iterator SList<T>::cend() const
	{
		return Const_Iterator{ *this };
	}

	template <typename T>
	void SList<T>::InsertAfter(Iterator& iterator, const T& data)
	{
		if (iterator._owner != this) throw std::runtime_error("Iterator doesn't belong to this SList");

		if (iterator._node == nullptr || iterator._node == _back)
		{
			PushBack(data);
		}
		else if (iterator._node != nullptr)
		{
			(iterator._node)->Next = new Node{ (iterator._node)->Next, data };
			++_size;
		}

	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityCheck)
	{
		Iterator it = begin();

		for (; it != end(); ++it)
		{
			if (equalityCheck(*it, value)) break;
		}

		return it;
	}

	template <typename T>
	typename SList<T>::Const_Iterator SList<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityCheck) const
	{
		Const_Iterator it = begin();

		for (; it != end(); ++it)
		{
			if (equalityCheck(*it, value)) break;
		}

		return it;
	}

	template <typename T>
	void SList<T>::Remove(const T& value, Iterator start, Iterator finish, std::function<bool(const T&, const T&)> equalityCheck)
	{
		if (start._owner != this) throw std::runtime_error("Start Iterator doesn't belong to this SList");
		if (finish._owner != this) throw std::runtime_error("Finish Iterator doesn't belong to this SList");

		Node* findernode{ start._node };

		if (findernode != nullptr && findernode == _front && equalityCheck(findernode->Data, value))
		{
			PopFront();
		}
		else
		{
			while (findernode != nullptr && findernode != finish._node && findernode->Next != nullptr)
			{
				if (equalityCheck((findernode->Next)->Data, value))
				{
					Node* toDelete{ findernode->Next };
					findernode->Next = toDelete->Next;
					delete toDelete;
					--_size;
					break;
				}

				findernode = findernode->Next;
			}
		}
	}

	template <typename T>
	void SList<T>::Remove(const T& value, std::function<bool(const T&, const T&)> equalityCheck)
	{
		Remove(value, begin(), end(), equalityCheck);
	}

	template <typename T>
	bool SList<T>::Remove(Iterator itr)
	{
		if (itr._owner != this) throw std::runtime_error("Iterator doesn't belong to this SList");

		bool success = false;
		if (itr._node == _front) { PopFront(); success = true; }
		else if (itr._node == _back) { PopBack(); success = true; }
		else
		{
			Node* nextNode{ (itr._node)->Next };

			(itr._node)->Data.~T();
			new(&((itr._node)->Data)) T(std::move(nextNode->Data));
			(itr._node)->Next = nextNode->Next;
			if (_back == nextNode) _back = itr._node;
			delete nextNode;

			--_size;
			success = true;
		}

		return success;
	}

	template <typename T>
	void SList<T>::RemoveAll(const T& value, Iterator start, Iterator finish, std::function<bool(const T&, const T&)> equalityCheck)
	{
		if (start._owner != this) throw std::runtime_error("Start Iterator doesn't belong to this SList");
		if (finish._owner != this) throw std::runtime_error("Finish Iterator doesn't belong to this SList");

		Node* findernode{ start._node };

		if (findernode != nullptr && findernode == _front && equalityCheck(findernode->Data, value))
		{
			findernode = findernode->Next;
			PopFront();
		}

		while (findernode != nullptr && findernode != finish._node && findernode->Next != nullptr)
		{
			while (equalityCheck((findernode->Next)->Data, value))
			{
				Node* toDelete{ findernode->Next };
				findernode->Next = toDelete->Next;
				delete toDelete;
				--_size;
			}

			findernode = findernode->Next;
		}
	}

	template <typename T>
	void SList<T>::RemoveAll(const T& value, std::function<bool(const T&, const T&)> equalityCheck)
	{
		RemoveAll(value, begin(), end(), equalityCheck);
	}
#pragma endregion

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}
}
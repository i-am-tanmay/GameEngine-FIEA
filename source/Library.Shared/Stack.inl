#include "Stack.h"

namespace FIEAGameEngine
{
	template<typename T>
	template <typename... Args>
	inline void Stack<T>::Emplace(Args&&... args)
	{
		_list.EmplaceFront(std::forward<Args>(args)...);
	}

	template <typename T>
	inline void Stack<T>::Push(const T& value)
	{
		_list.EmplaceFront(value);
	}

	template <typename T>
	inline void Stack<T>::Push(T&& value)
	{
		_list.EmplaceFront(std::move(value));
	}

	template <typename T>
	inline void Stack<T>::Pop()
	{
		_list.PopFront();
	}

	template <typename T>
	inline T& Stack<T>::Top()
	{
		return _list.Front();
	}

	template <typename T>
	inline const T& Stack<T>::Top() const
	{
		return _list.Front();
	}

	template <typename T>
	inline std::size_t Stack<T>::Size() const
	{
		return _list.Size();
	}

	template <typename T>
	inline bool Stack<T>::IsEmpty() const
	{
		return _list.IsEmpty();
	}

	template <typename T>
	inline void Stack<T>::Clear()
	{
		return _list.Clear();
	}
}
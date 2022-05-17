#pragma once

#include "SList.h"

namespace FIEAGameEngine
{
	template <typename T>
	class Stack final
	{
	public:
		template <typename... Args>
		void Emplace(Args&&... args);
		void Push(const T& value);
		void Push(T&& value);
		void Pop();
		T& Top();
		const T& Top() const;
		
		std::size_t Size() const;		
		bool IsEmpty() const;

		void Clear();

	private:
		SList<T> _list;
	};
}

#include "Stack.inl"
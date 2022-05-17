#pragma once
#include <cstdint>
#include <stdexcept>
#include <functional>
#include "DefaultIncrementFunct.h"
#include <iterator>
#include <type_traits>

namespace FIEAGameEngine
{
	template <typename T>
	class Vector final
	{
	public:

		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;

		/// <summary>
		/// Iterator for Vector
		/// </summary>
		class Iterator final
		{
			friend Vector;

		public:

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = value_type&;
			using pointer = value_type*;
			using iterator_category = std::bidirectional_iterator_tag;

			Iterator() = default;

			/// <summary>
			/// Constructor: Deep-copy from passed iterator
			/// </summary>
			/// <param name="other">Reference to Iterator to copy from</param>
			Iterator(const Iterator& other);

			/// <summary>
			/// Assignment: Deep-copy from passed iterator
			/// </summary>
			/// <param name="other">Reference to Iterator to copy from</param>
			/// <returns></returns>
			Iterator& operator=(const Iterator& other);

			Iterator(Iterator&& other) = default;
			Iterator& operator=(Iterator&& other) = default;
			~Iterator() = default;

			/// <summary>
			/// Check if two iterators are equal
			/// </summary>
			/// <param name="other">Reference to Iterator to check with</param>
			/// <returns>TRUE if both iterators point to the same things | FALSE if they point to different things</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Check if two iterators are not equal
			/// </summary>
			/// <param name="other">Reference to Iterator to check with</param>
			/// <returns>TRUE if both iterators point to the different things | FALSE if they point to the same things</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Pre-Increment: Points Iterator to the next Element of Vector
			/// </summary>
			/// <returns>Reference to the New Iterator</returns>
			Iterator& operator++();

			/// <summary>
			/// Post-Increment: Points Iterator to the next Element of Vector, but returns the current Iterator
			/// </summary>
			/// <param name=""></param>
			/// <returns>Reference to Iterator before incrementing</returns>
			Iterator operator++(int);

			/// <summary>
			/// Pre-Decrement: Points Iterator to the previous Element of Vector
			/// </summary>
			/// <returns>Reference to the New Iterator</returns>
			Iterator& operator--();

			/// <summary>
			/// Post-Decrement: Points Iterator to the previous Element of Vector, but returns the current Iterator
			/// </summary>
			/// <param name=""></param>
			/// <returns>Reference to Iterator before decrementing</returns>
			Iterator operator--(int);

			/// <summary>
			/// De-References Iterator to the Data it points to
			/// </summary>
			/// <returns>Reference to Data pointed to by the Iterator</returns>
			reference operator*() const;

		private:
			Iterator(const Vector& owner, size_type curIndex);
			const Vector* _owner{ nullptr };
			size_type _curIndex{ 0 };
		};

		class Const_Iterator final
		{
			friend Vector;

		public:

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = const value_type*;
			using iterator_category = std::bidirectional_iterator_tag;

			Const_Iterator() = default;

			/// <summary>
			/// Constructor: Deep-copy from passed iterator
			/// </summary>
			/// <param name="other">Reference to Iterator to copy from</param>
			Const_Iterator(const Iterator& other);

			/// <summary>
			/// Constructor: Deep-copy from passed const-iterator
			/// </summary>
			/// <param name="other">Reference to Const-Iterator to copy from</param>
			Const_Iterator(const Const_Iterator& other);

			/// <summary>
			/// Assignment: Deep-copy from passed iterator
			/// </summary>
			/// <param name="other">Reference to Iterator to copy from</param>
			/// <returns></returns>
			Const_Iterator& operator=(const Iterator& other);

			/// <summary>
			/// Assignment: Deep-copy from passed const-iterator
			/// </summary>
			/// <param name="other">Reference to Const-Iterator to copy from</param>
			/// <returns></returns>
			Const_Iterator& operator=(const Const_Iterator& other);

			Const_Iterator(Const_Iterator&& other) = default;
			Const_Iterator& operator=(Const_Iterator&& other) = default;
			~Const_Iterator() = default;

			/// <summary>
			/// Check if two const-iterators are equal
			/// </summary>
			/// <param name="other">Reference to Const-Iterator to check with</param>
			/// <returns>TRUE if both const-iterators point to the same things | FALSE if they point to different things</returns>
			bool operator==(const Const_Iterator& other) const;

			/// <summary>
			/// Check if two const-iterators are not equal
			/// </summary>
			/// <param name="other">Reference to Const-Iterator to check with</param>
			/// <returns>TRUE if both const-iterators point to the different things | FALSE if they point to the same things</returns>
			bool operator!=(const Const_Iterator& other) const;

			/// <summary>
			/// Pre-Increment: Points Const-Iterator to the next Element of Vector
			/// </summary>
			/// <returns>Reference to the New Const-Iterator</returns>
			Const_Iterator& operator++();

			/// <summary>
			/// Post-Increment: Points Const-Iterator to the next Element of Vector, but returns the current Const-Iterator
			/// </summary>
			/// <param name=""></param>
			/// <returns>Reference to Const-Iterator before incrementing</returns>
			Const_Iterator operator++(int);

			/// <summary>
			/// Pre-Decrement: Points Const-Iterator to the previous Element of Vector
			/// </summary>
			/// <returns>Reference to the New Const-Iterator</returns>
			Const_Iterator& operator--();

			/// <summary>
			/// Post-Decrement: Points Const-Iterator to the previous Element of Vector, but returns the current Iterator
			/// </summary>
			/// <param name=""></param>
			/// <returns>Reference to Const-Iterator before decrementing</returns>
			Const_Iterator operator--(int);

			/// <summary>
			/// De-References Const-Iterator to the Data it points to
			/// </summary>
			/// <returns>Constant Reference to Data pointed to by the Const-Iterator</returns>
			const_reference operator*() const;

		private:
			Const_Iterator(const Vector& owner, size_type curIndex);
			const Vector* _owner{ nullptr };
			size_type _curIndex{ 0 };
		};

		/// <summary>
		/// Constructor: Initialize Vector to Empty
		/// </summary>
		/// <param name="capacity">Reserve blocks of memory from start</param>
		Vector(size_type capacity = 0u);

		/// <summary>
		/// Constructor: Initialize Vector using List of Elements
		/// </summary>
		/// <param name="list">{List} of Elements</param>
		Vector(std::initializer_list<T> list);

		/// <summary>
		/// Constructor: Construct Vector using Elements from other Vector
		/// </summary>
		/// <param name="other">Reference to Vector to copy from</param>
		Vector(const Vector& other);

		/// <summary>
		/// Assignment: Deep-Copy Vector using other Vector
		/// </summary>
		/// <param name="other">Reference to Vector to copy from</param>
		/// <returns>Reference to this Vector</returns>
		Vector& operator=(const Vector& other);

		/// <summary>
		/// Constructor: Construct Vector by Moving all Elements of other vector to this
		/// </summary>
		/// <param name="other">Reference to Vector to move from</param>
		Vector(Vector&& other) noexcept;

		/// <summary>
		/// Assignment: Move all elements of other Vector to this Vector
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Reference to this Vector</returns>
		Vector& operator=(Vector&& other) noexcept;

		~Vector();

		/// <summary>
		/// Operator (+) overload for moving a vector into this vector
		/// </summary>
		/// <param name="other">Other vector this move into this</param>
		/// <returns>Reference to new this vector with members of other vector added at the end</returns>
		Vector& operator+(Vector&& other);

		/// <summary>
		/// Equality operator: Checks if contents of both vectors is same and in right order
		/// </summary>
		/// <param name="rhs">Reference to other vector to check with</param>
		/// <returns>TRUE if both vectors are equivalent | FALSE if not</returns>
		bool operator==(const Vector& rhs) const;

		/// <summary>
		/// NOT-Equality operator: Checks if contents of both vectors is not same or not in right order
		/// </summary>
		/// <param name="rhs">Reference to other vector to check with</param>
		/// <returns>TRUE if both vectors are not equivalent | FALSE if true</returns>
		bool operator!=(const Vector& rhs) const;

		/// <summary>
		/// Equality Operator for Data that is Pointers: Checks if contents of both vectors is same and in right order
		/// </summary>
		/// <param name="rhs">Reference to other vector to check with</param>
		/// <returns>TRUE if both vectors are equivalent | FALSE if not</returns>
		bool ComparePointerData(const Vector& rhs) const;

		/// <summary>
		/// De-Reference Vector to get Data at specified index
		/// </summary>
		/// <param name="index">Index of data to get</param>
		/// <returns>Reference to Data at spcified index</returns>
		reference operator[](size_type index);

		/// <summary>
		/// De-Reference Vector to get Data at specified index
		/// </summary>
		/// <param name="index">Index of data to get</param>
		/// <returns>Const-Reference to Data at spcified index</returns>
		const_reference operator[](size_type index) const;

		/// <summary>
		/// De-Reference Vector to get Data at specified index
		/// </summary>
		/// <param name="index">Index of data to get</param>
		/// <returns>Reference to Data at spcified index</returns>
		reference At(size_type index);

		/// <summary>
		/// De-Reference Vector to get Data at specified index
		/// </summary>
		/// <param name="index">Index of data to get</param>
		/// <returns>Const-Reference to Data at spcified index</returns>
		const_reference At(size_type index) const;

		/// <summary>
		/// Pre-allocate more memory
		/// </summary>
		/// <param name="capacity">Blocks of memory to reserve</param>
		/// <returns>TRUE if memory allocation was successful | FALSE if no memory was allocated</returns>
		bool Reserve(size_type capacity);

		/// <summary>
		/// Shrink Pre-allocated memory to fit current size of Vector
		/// </summary>
		/// <returns>TRUE if memory allocation was successful | FALSE if no memory was allocated</returns>
		bool ShrinkToFit();

		/// <summary>
		/// Shrink Pre-allocated memory to fit to specified capacity
		/// </summary>
		/// <param name="capacity">Blocks of memory to shrink to [should be more than current Size & less than current capacity]</param>
		/// <returns>TRUE if memory allocation was successful | FALSE if no memory was allocated</returns>
		bool ShrinkToFit(size_type capacity);

		/// <summary>
		/// Push new Element to back of Vector
		/// </summary>
		/// <param name="data">Reference of data to push</param>
		/// <param name="incrementFunctor">Function to increment using</param>
		/// <returns>Iterator to newly inserted element</returns>
		Iterator PushBack(const_reference data);

		/// <summary>
		/// Push new Element to back of Vector using Move Semantics
		/// </summary>
		/// <param name="data">Reference of data to push</param>
		/// <param name="incrementFunctor">Function to increment using</param>
		/// <returns>Iterator to newly inserted element</returns>
		Iterator PushBack(value_type&& data);

		/// <summary>
		/// Push new Element to back of Vector
		/// </summary>
		/// <param name="data">Reference of data to push</param>
		/// <param name="incrementFunctor">Function to increment using</param>
		/// <returns>Iterator to newly inserted element</returns>
		template<typename... Args>
		Iterator EmplaceBack(Args&&... args);

		/// <summary>
		/// Pop Element from back of Vector
		/// </summary>
		void PopBack();

		/// <summary>
		/// Does this Vector have any Elements
		/// </summary>
		/// <returns>TRUE if this Vector doesn't have any data | FALSE if it has data</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Get Count of Elements in Vector
		/// </summary>
		/// <returns>Number of Elements in this Vector</returns>
		size_type Size() const;

		/// <summary>
		/// Get Count of Pre-allocated memory blocks of Vector
		/// </summary>
		/// <returns>Number of Pre-allocated memory blocks in this Vector</returns>
		size_type Capacity() const;

		/// <summary>
		/// Get Data in Element at the front of Vector
		/// </summary>
		/// <returns>Reference to data in front Element</returns>
		reference Front();

		/// <summary>
		/// Get Data in Element at the front of Vector as a constant
		/// </summary>
		/// <returns>Constant Reference to data in front Element</returns>
		const_reference Front() const;

		/// <summary>
		/// Get Data in Element at the back of Vector
		/// </summary>
		/// <returns>Reference to data in back Element</returns>
		reference Back();

		/// <summary>
		/// Get Data in Element at the back of Vector as a constant
		/// </summary>
		/// <returns>Constant Reference to data in back Element</returns>
		const_reference Back() const;

		/// <summary>
		/// Get Iterator to start of the Vector
		/// </summary>
		/// <returns>Iterator to start of the Vector</returns>
		Iterator begin();

		/// <summary>
		/// Get Const-Iterator to start of the Vector
		/// </summary>
		/// <returns>Const-Iterator to start of the Vector</returns>
		Const_Iterator begin() const;

		/// <summary>
		/// Get Const-Iterator to start of the Vector
		/// </summary>
		/// <returns>Const-Iterator to start of the Vector</returns>
		Const_Iterator cbegin() const;

		/// <summary>
		/// Get Iterator to end of the Vector
		/// </summary>
		/// <returns>Iterator to end of the Vector</returns>
		Iterator end();

		/// <summary>
		/// Get Const-Iterator to end of the Vector
		/// </summary>
		/// <returns>Const-Iterator to end of the Vector</returns>
		Const_Iterator end() const;

		/// <summary>
		/// Get Const-Iterator to end of the Vector
		/// </summary>
		/// <returns>Const-Iterator to end of the Vector</returns>
		Const_Iterator cend() const;

		/// <summary>
		/// Deletes all Elements from Vector
		/// </summary>
		void Clear();

		/// <summary>
		/// Find if Data exists in Vector
		/// </summary>
		/// <param name="value">Reference to Data to find</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		/// <returns>Iterator pointing to Found Data</returns>
		Iterator Find(const_reference value, std::function<bool(const_reference, const_reference)> equalityCheck = std::equal_to<value_type>());

		/// <summary>
		/// Find if Data exists in Vector
		/// </summary>
		/// <param name="value">Reference to Data to find</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		/// <returns>Const-Iterator pointing to Found Data</returns>
		Const_Iterator Find(const_reference value, std::function<bool(const_reference, const_reference)> equalityCheck = std::equal_to<value_type>()) const;

		/// <summary>
		/// Remove the First Found Data Element from the Vector in the given range of Iterators
		/// </summary>
		/// <param name="value">Reference Data to Find</param>
		/// <param name="start">Iterator to where to start from [INCLUDING]</param>
		/// <param name="finish">Iterator to where to end on (EXCLUDING)</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		/// <returns>TRUE if an object was removed | FALSE if nothing was removed</returns>
		bool Remove(const_reference value, Iterator start, Iterator finish, std::function<bool(const_reference, const_reference)> equalityCheck = std::equal_to<value_type>());

		/// <summary>
		/// Remove the First Found Data Element from the whole Vector
		/// </summary>
		/// <param name="value">Reference Data to Find</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		/// <returns>TRUE if an object was removed | FALSE if nothing was removed</returns>
		bool Remove(const_reference value, std::function<bool(const_reference, const_reference)> equalityCheck = std::equal_to<value_type>());

		/// <summary>
		/// Remove the Elements pointed at by the Iterator
		/// </summary>
		/// <param name="itr">Iterator pointing to element to be deleted</param>
		/// <returns>TRUE if an object was removed | FALSE if nothing was removed</returns>
		bool Remove(Iterator itr);

		/// <summary>
		/// Remove All Instances of Found Data Elements from the Vector in the given range of Iterators
		/// </summary>
		/// <param name="value">Reference Data to Find</param>
		/// <param name="start">Iterator to where to start from [INCLUDING]</param>
		/// <param name="finish">Iterator to where to end on (EXCLUDING)</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		/// <returns>TRUE if an object was removed | FALSE if nothing was removed</returns>
		bool RemoveAll(const_reference value, Iterator start, Iterator finish, std::function<bool(const_reference, const_reference)> equalityCheck = std::equal_to<value_type>());

		/// <summary>
		/// Remove All Instances of Found Data Elements from the whole Vector
		/// </summary>
		/// <param name="value">Reference Data to Find</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		/// <returns>TRUE if an object was removed | FALSE if nothing was removed</returns>
		bool RemoveAll(const_reference value, std::function<bool(const_reference, const_reference)> equalityCheck = std::equal_to<value_type>());

		/// <summary>
		/// Remove All Elements within given range of Iterators
		/// </summary>
		/// <param name="start">Iterator to where to start from [INCLUDING]</param>
		/// <param name="finish">Iterator to where to end on (EXCLUDING)</param>
		/// <returns>TRUE if an object was removed | FALSE if nothing was removed</returns>
		bool RemoveAll(Iterator start, Iterator finish);

		/// <summary>
		/// Set New Increment Scheme for Vector
		/// </summary>
		/// <param name="_incrementFunctor">New increment functor</param>
		void SetIncrementFunctor(std::function<size_type(size_type, size_type)> incrementFunctor);

	private:
		pointer _baseData{ nullptr };
		size_type _size{ 0u };
		size_type _capacity{ 0u };

		std::function<size_type(size_type, size_type)> _incrementFunctor{ DefaultIncrementFunct{} };
	};

}

#include "Vector.inl"
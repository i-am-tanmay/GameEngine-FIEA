#pragma once
#include <cstdint>
#include <stdexcept>
#include <functional>

namespace FIEAGameEngine
{
	template <typename T>
	class SList final
	{

	private:
		struct Node final
		{
		public:
			T Data;
			Node* Next;

			template<typename... Args>
			Node(Node* next, Args&&... args) : Data{ std::forward<Args>(args)... }, Next{ next } {};
		};


	public:

		/// <summary>
		/// Iterator for SList
		/// </summary>
		class Iterator final
		{
			friend SList;

		public:
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
			/// Pre-Increment: Points Iterator to the next Node of the SList
			/// </summary>
			/// <returns>Reference to the New Iterator</returns>
			Iterator& operator++();

			/// <summary>
			/// Post-Increment: Points Iterator to the next Node of the SList, but returns the current Iterator
			/// </summary>
			/// <param name=""></param>
			/// <returns>Reference to Iterator before incrementing</returns>
			Iterator operator++(int);

			/// <summary>
			/// De-References Iterator to the Data it points to
			/// </summary>
			/// <returns>Reference to Data pointed to by the Iterator</returns>
			T& operator*() const;

		private:
			Iterator(const SList& owner, Node* node = nullptr);
			const SList* _owner{ nullptr };
			Node* _node{ nullptr };
		};

		class Const_Iterator final
		{
			friend SList;

		public:
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
			/// Pre-Increment: Points Const-Iterator to the next Node of the SList
			/// </summary>
			/// <returns>Reference to the New Const-Iterator</returns>
			Const_Iterator& operator++();

			/// <summary>
			/// Post-Increment: Points Const-Iterator to the next Node of the SList, but returns the current Const-Iterator
			/// </summary>
			/// <param name=""></param>
			/// <returns>Reference to Const-Iterator before incrementing</returns>
			Const_Iterator operator++(int);

			/// <summary>
			/// De-References Const-Iterator to the Data it points to
			/// </summary>
			/// <returns>Constant Reference to Data pointed to by the Const-Iterator</returns>
			const T& operator*() const;

		private:
			Const_Iterator(const SList& owner, Node* node = nullptr);
			const SList* _owner{ nullptr };
			Node* _node{ nullptr };
		};


		/// <summary>
		/// Constructor: Initialize Singly Linked List to Empty
		/// </summary>
		SList();

		/// <summary>
		/// Constructor: Construct Linked List using Nodes from other Linked List
		/// </summary>
		/// <param name="other">Reference to Linked List to copy from</param>
		SList(const SList& other);

		/// <summary>
		/// Assignment: Deep-Copy Linked List using other Linked List
		/// </summary>
		/// <param name="other">Reference to Linked List to copy from</param>
		/// <returns>Reference to this Linked List</returns>
		SList& operator=(const SList& other);

		SList(SList&& other) = delete;
		SList& operator=(SList&& other) = delete;
		~SList();

		/// <summary>
		/// Push new Node onto front of Linked List
		/// </summary>
		/// <param name="data">Reference of data to push</param>
		Iterator PushFront(const T& data);

		/// <summary>
		/// Push Move new Node onto front of Linked List
		/// </summary>
		/// <param name="data">Reference of data to move</param>
		Iterator PushFront(T&& data);

		/// <summary>
		/// Emplace new Node onto front of Linked List
		/// </summary>
		/// <param name="data">Reference of data to push</param>
		template<typename... Args>
		Iterator EmplaceFront(Args&&... args);

		/// <summary>
		/// Pop Node from front of Linked List
		/// </summary>
		void PopFront();

		/// <summary>
		/// Push new Node to back of Linked List
		/// </summary>
		/// <param name="data">Reference of data to push</param>
		Iterator PushBack(const T& data);

		/// <summary>
		/// Push Move new Node to back of Linked List
		/// </summary>
		/// <param name="data">Reference of data to move</param>
		Iterator PushBack(T&& data);

		/// <summary>
		/// Emplace new Node to back of Linked List
		/// </summary>
		/// <param name="data">Reference of data to push</param>
		template<typename... Args>
		Iterator EmplaceBack(Args&&... args);

		/// <summary>
		/// Pop Node from back of Linked List
		/// </summary>
		void PopBack();

		/// <summary>
		/// Does this Linked List have any Nodes
		/// </summary>
		/// <returns>TRUE if this linked list doesn't have any data | FALSE if it has data</returns>
		inline bool IsEmpty() const;

		/// <summary>
		/// Get Count of Nodes in Linked List
		/// </summary>
		/// <returns>Number of Nodes in this Linked List</returns>
		const std::size_t Size() const;

		/// <summary>
		/// Get Data in Node at the front of Linked List
		/// </summary>
		/// <returns>Reference to data in front Node</returns>
		T& Front();

		/// <summary>
		/// Get Data in Node at the front of Linked List as a constant
		/// </summary>
		/// <returns>Constant Reference to data in front Node</returns>
		const T& Front() const;

		/// <summary>
		/// Get Data in Node at the back of Linked List
		/// </summary>
		/// <returns>Reference to data in back Node</returns>
		T& Back();

		/// <summary>
		/// Get Data in Node at the back of Linked List as a constant
		/// </summary>
		/// <returns>Constant Reference to data in back Node</returns>
		const T& Back() const;

		/// <summary>
		/// Deletes all Nodes from Linked List
		/// </summary>
		void Clear();

		/// <summary>
		/// Get Iterator to start of the SList
		/// </summary>
		/// <returns>Iterator to start of the SList</returns>
		Iterator begin();

		/// <summary>
		/// Get Const-Iterator to start of the SList
		/// </summary>
		/// <returns>Const-Iterator to start of the SList</returns>
		Const_Iterator begin() const;

		/// <summary>
		/// Get Const-Iterator to start of the SList
		/// </summary>
		/// <returns>Const-Iterator to start of the SList</returns>
		Const_Iterator cbegin() const;

		/// <summary>
		/// Get Iterator to end of the SList
		/// </summary>
		/// <returns>Iterator to end of the SList</returns>
		Iterator end();

		/// <summary>
		/// Get Const-Iterator to end of the SList
		/// </summary>
		/// <returns>Const-Iterator to end of the SList</returns>
		Const_Iterator end() const;

		/// <summary>
		/// Get Const-Iterator to end of the SList
		/// </summary>
		/// <returns>Const-Iterator to end of the SList</returns>
		Const_Iterator cend() const;

		/// <summary>
		/// Inserts new data after the Node the passed Iterator is pointing to
		/// </summary>
		/// <param name="iterator">Reference to Iterator to Insert new data after</param>
		/// <param name="data">Reference to Data to be pushed</param>
		void InsertAfter(Iterator& iterator, const T& data);

		/// <summary>
		/// Find if Data exists in SList
		/// </summary>
		/// <param name="value">Reference to Data to find</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		/// <returns>Iterator pointing to Found Data</returns>
		Iterator Find(const T& value, std::function<bool(const T&, const T&)> equalityCheck = std::equal_to<T>());

		/// <summary>
		/// Find if Data exists in SList
		/// </summary>
		/// <param name="value">Reference to Data to find</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		/// <returns>Const-Iterator pointing to Found Data</returns>
		Const_Iterator Find(const T& value, std::function<bool(const T&, const T&)> equalityCheck = std::equal_to<T>()) const;

		/// <summary>
		/// Remove the First Found Data Node from the SList in the given range of Iterators
		/// </summary>
		/// <param name="value">Reference Data to Find</param>
		/// <param name="start">Iterator to where to start from [INCLUDING]</param>
		/// <param name="finish">Iterator to where to end on (EXCLUDING)</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		void Remove(const T& value, Iterator start, Iterator finish, std::function<bool(const T&, const T&)> equalityCheck = std::equal_to<T>());

		/// <summary>
		/// Remove the First Found Data Node from the whole SList
		/// </summary>
		/// <param name="value">Reference Data to Find</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		void Remove(const T& value, std::function<bool(const T&, const T&)> equalityCheck = std::equal_to<T>());

		/// <summary>
		/// Remove Node pointed by the passed Iterator
		/// </summary>
		/// <param name="itr">Iterator to node to delete</param>
		/// <returns>TRUE if node was removed | FALSE if nothing was removed</returns>
		bool Remove(Iterator itr);

		/// <summary>
		/// Remove All Instances of Found Data Nodes from the SList in the given range of Iterators
		/// </summary>
		/// <param name="value">Reference Data to Find</param>
		/// <param name="start">Iterator to where to start from [INCLUDING]</param>
		/// <param name="finish">Iterator to where to end on (EXCLUDING)</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		void RemoveAll(const T& value, Iterator start, Iterator finish, std::function<bool(const T&, const T&)> equalityCheck = std::equal_to<T>());

		/// <summary>
		/// Remove All Instances of Found Data Nodes from the whole SList
		/// </summary>
		/// <param name="value">Reference Data to Find</param>
		/// <param name="equalityCheck">Functor to check equality of Data</param>
		void RemoveAll(const T& value, std::function<bool(const T&, const T&)> equalityCheck = std::equal_to<T>());


	private:
		std::size_t _size{ 0u };
		Node* _front{ nullptr };
		Node* _back{ nullptr };

	};
}

#include "SList.inl"
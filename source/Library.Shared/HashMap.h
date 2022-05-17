#pragma once
#include <cstdint>
#include <stdexcept>
#include <functional>

#include "DefaultHashFunct.h"
#include "SList.h"
#include "Vector.h"

namespace FIEAGameEngine
{
	template <typename TKey, typename TData>
	class HashMap final
	{
	public:

		using _TPair = std::pair<const TKey, TData>;
		using _TList = SList<_TPair>;
		using _TMap = Vector<_TList>;

		using size_type = std::size_t;
		using value_type = _TPair;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;

		using HashFunct = std::function<size_type(TKey)>;
		using EqualityFunct = std::function<bool(const TKey&, const TKey&)>;

		/// <summary>
		/// Iterator for HashMap
		/// </summary>
		class Iterator final
		{
			friend HashMap;

		public:

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = _TPair;
			using reference = value_type&;
			using pointer = value_type*;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default Constructor for HashMap Iterator
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Copy Constructor for HashMap Iterator
			/// </summary>
			/// <param name="other">Other iterator to copy from</param>
			Iterator(const Iterator& other) = default;

			/// <summary>
			/// Copy Assignment for HashMap Iterator
			/// </summary>
			/// <param name="other">Other iterator to copy from</param>
			/// <returns>Reference to this Iterator (changed)</returns>
			Iterator& operator=(const Iterator& other) = default;

			/// <summary>
			/// Move Constructor for HashMap Iterator
			/// </summary>
			/// <param name="other">Other iterator to move from</param>
			Iterator(Iterator&& other) = default;

			/// <summary>
			/// Move Assignment for HashMap Iterator
			/// </summary>
			/// <param name="other">Other iterator to move from</param>
			/// <returns>Reference to this Iterator (changed)</returns>
			Iterator& operator=(Iterator&& other) = default;
			~Iterator() = default;

			/// <summary>
			/// Check if iterators are equal
			/// </summary>
			/// <param name="other">Other iterator to check with</param>
			/// <returns>TRUE if they point to same thing | FALSE if they point to different things</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Check if iterators are not equal
			/// </summary>
			/// <param name="other">Other iterator to check with</param>
			/// <returns>TRUE if they point to different things | FALSE if they point to same things</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Pre-Increment for Iterator -> Points to the next element
			/// </summary>
			/// <returns></returns>
			Iterator& operator++();

			/// <summary>
			/// Post-Increment for Iterator -> Points to the next element
			/// </summary>
			/// <param name=""></param>
			/// <returns>Iterator before incrementing</returns>
			Iterator operator++(int);

			/// <summary>
			/// De-Reference Operator
			/// </summary>
			/// <returns>Reference to value</returns>
			reference operator*() const;

			/// <summary>
			/// Arrow dereference Operator
			/// </summary>
			/// <returns>Pointer to value</returns>
			pointer operator->() const;

		private:
			Iterator(HashMap& owner, size_type listNum, typename _TList::Iterator itrElement);
			HashMap* _owner{ nullptr };
			size_type _listNum{ 0u };
			typename _TList::Iterator _itrElement;
		};

		/// <summary>
		/// Const_Iterator for HashMap
		/// </summary>
		class Const_Iterator final
		{
			friend HashMap;

		public:

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = _TPair;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type*;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default Constructor for HashMap Const_Iterator
			/// </summary>
			Const_Iterator() = default;

			/// <summary>
			/// Copy Constructor for HashMap Const_Iterator
			/// </summary>
			/// <param name="other">Other Iterator to copy from</param>
			Const_Iterator(const Iterator& other);

			/// <summary>
			/// Copy Constructor for HashMap Const_Iterator
			/// </summary>
			/// <param name="other">Other Const_Iterator to copy from</param>
			Const_Iterator(const Const_Iterator& other) = default;

			/// <summary>
			/// Copy Assignment for HashMap Const_Iterator
			/// </summary>
			/// <param name="other">Other iterator to copy from</param>
			/// <returns>Reference to this Const_Iterator (changed)</returns>
			Const_Iterator& operator=(const Iterator& other);

			/// <summary>
			/// Copy Assignment for HashMap Const_Iterator
			/// </summary>
			/// <param name="other">Other Const_Iterator to copy from</param>
			/// <returns>Reference to this Const_Iterator (changed)</returns>
			Const_Iterator& operator=(const Const_Iterator& other) = default;

			/// <summary>
			/// Move Constructor for HashMap Const_Iterator
			/// </summary>
			/// <param name="other">Other Const_Iterator to move from</param>
			Const_Iterator(Const_Iterator&& other) = default;

			/// <summary>
			/// Move Assignment for HashMap Const_Iterator
			/// </summary>
			/// <param name="other">Other Const_Iterator to move from</param>
			/// <returns>Reference to this Const_Iterator (changed)</returns>
			Const_Iterator& operator=(Const_Iterator&& other) = default;
			~Const_Iterator() = default;

			/// <summary>
			/// Check if Const_Iterator's are equal
			/// </summary>
			/// <param name="other">Other Const_Iterator to check with</param>
			/// <returns>TRUE if they point to same thing | FALSE if they point to different things</returns>
			bool operator==(const Const_Iterator& other) const;

			/// <summary>
			/// Check if Const_Iterator's are not equal
			/// </summary>
			/// <param name="other">Other Const_Iterator to check with</param>
			/// <returns>TRUE if they point to different things | FALSE if they point to same things</returns>
			bool operator!=(const Const_Iterator& other) const;

			/// <summary>
			/// Pre-Increment for Const_Iterator -> Points to the next element
			/// </summary>
			/// <returns></returns>
			Const_Iterator& operator++();

			/// <summary>
			/// Post-Increment for Const_Iterator -> Points to the next element
			/// </summary>
			/// <param name=""></param>
			/// <returns>Iterator before incrementing</returns>
			Const_Iterator operator++(int);

			/// <summary>
			/// De-Reference Operator
			/// </summary>
			/// <returns>Reference to value</returns>
			const_reference operator*() const;

			/// <summary>
			/// Arrow dereference Operator
			/// </summary>
			/// <returns>Pointer to value</returns>
			const_pointer operator->() const;

		private:
			Const_Iterator(const HashMap& owner, size_type listNum, typename _TList::Const_Iterator itrElement);
			const HashMap* _owner{ nullptr };
			size_type _listNum{ 0 };
			typename _TList::Const_Iterator _itrElement;
		};

		/// <summary>
		/// Default Constructor for HashMap
		/// </summary>
		/// <param name="capacity">Number of Chains [CANNOT BE LESS THAN 2]</param>
		/// <param name="hashFunct">Hash Function to hash the key with</param>
		/// <param name="equalityFunct">Equality Function to check IF 2 Keys are equal</param>
		HashMap(size_type capacity = 17u, HashFunct hashFunct = DefaultHashFunct<TKey>{}, EqualityFunct equalityFunct = std::equal_to<TKey>{});
		
		/// <summary>
		/// Constructor: Initialize HashMap using List of Elements
		/// </summary>
		/// <param name="list">{List} of Elements</param>
		HashMap(std::initializer_list<_TPair> list);

		/// <summary>
		/// Copy Constructor for HashMap
		/// </summary>
		/// <param name="other">Other HashMap to copy from</param>
		HashMap(const HashMap& other) = default;

		/// <summary>
		/// Copy Assignment for HashMap
		/// </summary>
		/// <param name="other">Other HashMap to copy from</param>
		/// <returns>Reference to this HashMap after copy</returns>
		HashMap& operator=(const HashMap& other) = default;

		/// <summary>
		/// Move Constructor for HashMap [NOTE: Moved HashMap is re-usable after move]
		/// </summary>
		/// <param name="other">Other HashMap to move from</param>
		HashMap(HashMap&& other) noexcept;

		/// <summary>
		/// Move Assignment for HashMap [NOTE: Moved HashMap is re-usable after move]
		/// </summary>
		/// <param name="other">Other HashMap to move from</param>
		/// <returns>Reference to this HashMap after move</returns>
		HashMap& operator=(HashMap&& other) noexcept;
		~HashMap() = default;

		/// <summary>
		/// Start Iterator for HashMap
		/// </summary>
		/// <returns>Iterator pointing to First element of the HashMap</returns>
		Iterator begin();

		/// <summary>
		/// Start Const_Iterator for HashMap
		/// </summary>
		/// <returns>Const_Iterator pointing to First element of the HashMap</returns>
		Const_Iterator begin() const;

		/// <summary>
		/// Start Const_Iterator for HashMap
		/// </summary>
		/// <returns>Const_Iterator pointing to First element of the HashMap</returns>
		Const_Iterator cbegin() const;

		/// <summary>
		/// End Iterator for HashMap
		/// </summary>
		/// <returns>Iterator pointing to Last element of the HashMap</returns>
		Iterator end();

		/// <summary>
		/// End Const_Iterator for HashMap
		/// </summary>
		/// <returns>Const_Iterator pointing to Last element of the HashMap</returns>
		Const_Iterator end() const;

		/// <summary>
		/// End Const_Iterator for HashMap
		/// </summary>
		/// <returns>Const_Iterator pointing to Last element of the HashMap</returns>
		Const_Iterator cend() const;

		/// <summary>
		/// Current Filled Size of HashMap
		/// </summary>
		/// <returns>Current No. of Elements in the HashMap</returns>
		size_type Size() const;

		/// <summary>
		/// No. of Buckets of the HashMap
		/// </summary>
		/// <returns>Count of buckets of the hashmap</returns>
		size_type Capacity() const;

		/// <summary>
		/// Are there any elements in the HashMap?
		/// </summary>
		/// <returns>TRUE if there's atleast 1 element in the HashMap | FALSE if there's 0 elements</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Returns Fill-Factor for the HashMap
		/// </summary>
		/// <param name="threshold">Minimum number of elements in a Chain to be considered as Filled</param>
		/// <returns>filled buckets / total buckets</returns>
		float LoadFactor(size_type threshold = 0u);

		/// <summary>
		/// Find location of Key in HashMap
		/// </summary>
		/// <param name="key">Reference to Key to Find</param>
		/// <returns>Iterator to Element if Key exists in HashMap | End-Iterator if key doesn't exist</returns>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Find location of Key in HashMap
		/// </summary>
		/// <param name="key">Reference to Key to Find</param>
		/// <returns>Const_Iterator to Element if Key exists in HashMap | End-Const_Iterator if key doesn't exist</returns>
		Const_Iterator Find(const TKey& key) const;

		/// <summary>
		/// Does this Key exists in this HashMap?
		/// </summary>
		/// <param name="key">Reference to Key to Find</param>
		/// <returns>TRUE if key exists | FALSE if key not in HashMap</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Does this Key exists in this HashMap? [Also sets reference to data in a pointer if Key exists]
		/// </summary>
		/// <param name="key">Reference to Key to Find</param>
		/// <param name="data">Pointer to Data is set if key exists</param>
		/// <returns>TRUE if key exists | FALSE if key not in HashMap</returns>
		bool ContainsKey(const TKey& key, TData*& data);

		/// <summary>
		/// Does this Key exists in this HashMap? [Also sets reference to data in a constant pointer if Key exists]
		/// </summary>
		/// <param name="key">Reference to Key to Find</param>
		/// <param name="data">Constant Pointer to Data is set if key exists</param>
		/// <returns>TRUE if key exists | FALSE if key not in HashMap</returns>
		bool ContainsKey(const TKey& key, const TData*& data) const;

		/// <summary>
		/// Insert element in HashMap
		/// </summary>
		/// <param name="datapair">Pair of Key & Value to insert</param>
		/// <returns>Pair of Iterator pointing to inserted value & bool if Insert was successful</returns>
		std::pair<Iterator, bool> Insert(const _TPair& datapair);

		/// <summary>
		/// Move Insert element in HashMap
		/// </summary>
		/// <param name="datapair">Pair of Key & Value to insert</param>
		/// <returns>Pair of Iterator pointing to inserted value & bool if Insert was successful</returns>
		std::pair<Iterator, bool> Insert(_TPair&& datapair);

		/// <summary>
		/// Insert element in HashMap
		/// </summary>
		/// <param name="key">Key to insert</param>
		/// <param name="data">Value to insert</param>
		/// <returns>Pair of Iterator pointing to inserted value & bool if Insert was successful</returns>
		std::pair<Iterator, bool> Insert(const TKey& key, const TData& data);

		/// <summary>
		/// Returns reference to data of given Key
		/// </summary>
		/// <param name="key">Key to return data of</param>
		/// <returns>Reference to data</returns>
		TData& At(const TKey& key);

		/// <summary>
		/// Returns constant reference to data of given Key
		/// </summary>
		/// <param name="key">Key to return data of</param>
		/// <returns>Constant Reference to data</returns>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// [] Operator to get value at given key if it exists, otherwise construct one [NOTE: Object should have Default Constructor to be able to construct new data]
		/// </summary>
		/// <param name="key">Reference to Key to Find</param>
		/// <returns>Reference to Data at given key</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Remove given element from the list using Key
		/// </summary>
		/// <param name="key">Reference to key to remove</param>
		/// <returns>TRUE if element was removed | FALSE if nothing was removed / not found</returns>
		bool Remove(const TKey& key);

		/// <summary>
		/// Deletes all elements from the HashMap
		/// </summary>
		void Clear();

		/// <summary>
		/// Set new capacity (No. of Chains) of the HashMap
		/// </summary>
		/// <param name="capacity">New capacity [CANNOT BE LESS THAN 2]</param>
		/// <returns>TRUE if rehash successful | FALSE if couldn't rehash</returns>
		bool ReHash(size_type capacity);

		/// <summary>
		/// Set new Hash Function for the HashMap
		/// </summary>
		/// <param name="newHashFunct">Functor to new hash function</param>
		void ReHash(HashFunct newHashFunct);

		/// <summary>
		/// Set new Equality Function for the HashMap
		/// </summary>
		/// <param name="newEqualityFunct">Functor to new equality function</param>
		void ReHash(EqualityFunct newEqualityFunct);

		/// <summary>
		/// Get current Hash Functor
		/// </summary>
		/// <returns>Hash Functor of this HashMap</returns>
		HashFunct GetHashFunct() const;

		/// <summary>
		/// Get current Equality Functor
		/// </summary>
		/// <returns>Equality Functor of this HashMap</returns>
		EqualityFunct GetEqualityFunct() const;

	private:
		_TMap _map{ 17u };
		size_type _count{ 0u };
		HashFunct _hashFunct{ DefaultHashFunct<TKey>{} };
		EqualityFunct _equalityFunct{ std::equal_to<TKey>{} };
	};
}

#include "HashMap.inl"
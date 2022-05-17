#pragma once
#include <cstdint>
#include <stdexcept>
#include <functional>
#include <gsl/gsl>
#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"
#include "RTTI.h"
#include "Factory.h"
#include "DefaultHashFunct.h"

namespace FIEAGameEngine
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope);

	public:
		using _HashMapSignature = HashMap<std::string, Datum>;
		using _TPair = typename _HashMapSignature::_TPair;
		using HashFunct = typename _HashMapSignature::HashFunct;
		using EqualityFunct = typename _HashMapSignature::EqualityFunct;

		using size_type = std::size_t;

	public:

		/// <summary>
		/// Constructor for Scope
		/// </summary>
		/// <param name="capacity">Capacity for Order Table</param>
		/// <param name="hashmap_size">Capacity for Map</param>
		/// <param name="hashFunct">Hash Function for Map</param>
		/// <param name="equalityFunct">Function to check equality for Map's Key</param>
		Scope(size_type capacity = 0u, size_type hashmap_size = 17u, HashFunct hashFunct = DefaultHashFunct<std::string>{}, EqualityFunct equalityFunct = std::equal_to<std::string>{});

		virtual ~Scope();

		/// <summary>
		/// Copy Contructor for Scope
		/// </summary>
		/// <param name="rhs">Reference to other scope to copy from</param>
		Scope(const Scope& rhs);

		/// <summary>
		/// Copy Assignment for Scope
		/// </summary>
		/// <param name="rhs">Reference to other scope to copy from</param>
		/// <returns>Reference to this scope after copy</returns>
		Scope& operator=(const Scope& rhs);

		/// <summary>
		/// Move Constructor for Scope
		/// </summary>
		/// <param name="rhs">Reference to other scope to move</param>
		Scope(Scope&& rhs) noexcept;

		/// <summary>
		/// Move Assignment for Scope
		/// </summary>
		/// <param name="rhs">Reference to other scope to move</param>
		/// <returns>Reference to this scope after move</returns>
		Scope& operator=(Scope&& rhs) noexcept;

		/// <summary>
		/// Adds a new entry in this Scope Table
		/// </summary>
		/// <param name="name">Name for entry</param>
		/// <returns>Reference to the added entry</returns>
		Datum& Append(const std::string& name, DatumTypes type = DatumTypes::Unknown, size_type capacity = 0);

		/// <summary>
		/// Adds a new entry in this Scope Table
		/// </summary>
		/// <param name="name"></param>
		/// <param name="datum"></param>
		/// <returns></returns>
		Datum& Append(const std::string& name, const Datum& datum);

		/// <summary>
		/// Adds a new scope table entry in this scope table
		/// </summary>
		/// <param name="name">Name for entry</param>
		/// <param name="capacity">Capacity for new scope's Order Table</param>
		/// <param name="hashmap_size">Capacity for new scope's Map</param>
		/// <param name="hashFunct">Hash Function for new scope's Map</param>
		/// <param name="equalityFunct">Function to check equality for new scope's Map's Key</param>
		/// <returns>Reference to the added scope table entry</returns>
		Scope& AppendScope(const std::string& name, size_type capacity = 0u, size_type hashmap_size = 17u, HashFunct hashFunct = DefaultHashFunct<std::string>{}, EqualityFunct equalityFunct = std::equal_to<std::string>{});

		/// <summary>
		/// Adopt a scope table to this scope table
		/// </summary>
		/// <param name="scope">Reference to scope to adopt</param>
		/// <param name="name">Name for entry of table</param>
		void Adopt(Scope& scope, const std::string& name);

		/// <summary>
		/// Remove the parent of this scope
		/// </summary>
		void RemoveParent();

		/// <summary>
		/// Get parent scope table of this scope
		/// </summary>
		/// <returns>Pointer to this scope</returns>
		Scope* GetParent();

		/// <summary>
		/// Get parent constant scope table of this scope
		/// </summary>
		/// <returns>Const Pointer to this scope</returns>
		const Scope* GetParent() const;

		/// <summary>
		/// Get Entry from given of given name | Add new if doesn't exist
		/// </summary>
		/// <param name="name">Name of entry</param>
		/// <returns>Reference to entry at given name</returns>
		Datum& operator[](std::string name);

		/// <summary>
		/// Get Entry from table at given index
		/// </summary>
		/// <param name="index">Index of entry</param>
		/// <returns>Reference to entry at given index</returns>
		Datum& operator[](size_type index);

		/// <summary>
		/// Get Const Entry from table at given index
		/// </summary>
		/// <param name="index">Index of entry</param>
		/// <returns>Const Reference to entry at given index</returns>
		const Datum& operator[](size_type index) const;

		/// <summary>
		/// Equality check for scopes
		/// </summary>
		/// <param name="rhs">Reference to other scope table to check with</param>
		/// <returns>TRUE if both scope tables have same order and same data | FALSE if not either</returns>
		bool operator==(const Scope& rhs) const;

		/// <summary>
		/// Number of entries in the table
		/// </summary>
		/// <returns>Size of this table</returns>
		size_type Size() const;

		/// <summary>
		/// NOT-Equality check for scopes
		/// </summary>
		/// <param name="rhs">Reference to other scope table to check with</param>
		/// <returns>TRUE if both scope tables have different order and different data | FALSE if same</returns>
		bool operator!=(const Scope& rhs) const;

		/// <summary>
		/// Get Datum of Found Key
		/// </summary>
		/// <param name="">Key to find datum of</param>
		/// <returns>Reference to datum at given key</returns>
		Datum& At(const std::string&);

		/// <summary>
		/// Get Const Datum of Found Key
		/// </summary>
		/// <param name="">Key to find const datum of</param>
		/// <returns>Reference to const datum at given key</returns>
		const Datum& At(const std::string&) const;

		/// <summary>
		/// Find an Entry in this scope table of given name (Can also be able to find in children)
		/// </summary>
		/// <param name="name">Name of entry</param>
		/// <param name="findInChildren">TRUE to go through all nested children to find entry | FALSE if only limited to local scope table</param>
		/// <param name="foundScope">Reference to Pointer, which points to Scope containing the found entry</param>
		/// <returns>Pointer to found entry | nullptr if not found</returns>
		Datum* Find(const std::string& name, bool findInChildren = false, Scope** foundScope = nullptr);

		/// <summary>
		/// Find a Const Entry in this scope table of given name (Can also be able to find in children)
		/// </summary>
		/// <param name="name">Name of entry</param>
		/// <param name="findInChildren">TRUE to go through all nested children to find entry | FALSE if only limited to local scope table</param>
		/// <param name="foundScope">Reference to Pointer, which points to Scope containing the found entry</param>
		/// <returns>Pointer to found const entry | nullptr if not found</returns>
		const Datum* Find(const std::string& name, bool findInChildren = false, const Scope** foundScope = nullptr) const;

		/// <summary>
		/// Find if a scope table exists within this scope table
		/// </summary>
		/// <param name="scope">Reference to other scope find</param>
		std::pair<Datum*, size_type> FindContainedScope(const Scope& scope) const;

		/// <summary>
		/// Search entry in upwards hierarchy of the scope
		/// </summary>
		/// <param name="name">Name of entry to search</param>
		/// <returns>Pointer to entry of given name</returns>
		Datum* Search(const std::string& name);

		/// <summary>
		/// Search const entry in upwards hierarchy of the scope
		/// </summary>
		/// <param name="name">Name of entry to search</param>
		/// <returns>Pointer to const entry of given name</returns>
		const Datum* Search(const std::string& name) const;

		/// <summary>
		/// Search entry in upwards hierarchy of the scope
		/// </summary>
		/// <param name="name">Name of entry to search</param>
		/// <param name="foundScope">Pointer which points to Scope containing the found entry</param>
		/// <returns>Pointer to entry of given name</returns>
		Datum* Search(const std::string& name, Scope*& foundScope);

		/// <summary>
		/// Search const entry in upwards hierarchy of the scope
		/// </summary>
		/// <param name="name">Name of entry to search</param>
		/// <param name="foundScope">Pointer which points to Scope containing the found entry</param>
		/// <returns>Pointer to const entry of given name</returns>
		const Datum* Search(const std::string& name, const Scope*& foundScope) const;

		/// <summary>
		/// Scope Table to string overload
		/// </summary>
		/// <returns>String for this scope table</returns>
		std::string ToString() const override;

	public:
		bool Equals(const RTTI* rhs) const override
		{
			if (rhs != nullptr && rhs->Is(Scope::TypeIdClass()))
				return operator==(*(reinterpret_cast<const Scope*>(rhs)));

			return false;
		}
		virtual bool Equals(const Scope* rhs) const
		{
			if (rhs == nullptr) return false;
			return operator==(*rhs);
		}

	protected:
		virtual void Clear();

		virtual gsl::owner<Scope*> Clone() const;

		bool IsRecursiveChildOf(const Scope& scope) const;
		bool IsRecursiveParentOf(const Scope& scope) const;

	protected:
		Scope* _parent{ nullptr };
		_HashMapSignature _hashmap{ 0 };
		Vector<_TPair*> _orderTable{ 0 };
	};
}
#pragma once
#include <cstdint>
#include <stdexcept>
#include <functional>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "DefaultIncrementFunct.h"
#include "HashMap.h"

using namespace std::string_literals;
namespace FIEAGameEngine
{
	class RTTI;
	class Scope;

	enum class DatumTypes
	{
		Unknown,
		Integer,
		Float,
		Vector,
		Matrix,
		String,
		Pointer,
		ScopeTable,

		Begin = Integer,
		End = ScopeTable
	};

	class Datum final
	{
	public:
		friend class Scope;
		friend class Attributed;

		using size_type = std::size_t;

	private:
		union DatumVals final
		{
			std::uint8_t* _byte;
			std::int32_t* _int;
			float* _float;
			glm::vec4* _vec;
			glm::mat4* _mat;
			std::string* _string;
			RTTI** _rtti;
			Scope** _scope;
			void* _voidp{ nullptr };
		};

	public:

		/// <summary>
		/// Constructor for Datum
		/// </summary>
		/// <param name="type">Type of datum</param>
		/// <param name="capacity">Capacity to Reserve</param>
		Datum(DatumTypes type = DatumTypes::Unknown, size_type capacity = 0);
		~Datum();

		/// <summary>
		/// Deep-Copy Constructor
		/// </summary>
		/// <param name="">Reference to Datum to copy from</param>
		Datum(const Datum&);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="">Reference to Datum to move from</param>
		Datum(Datum&&) noexcept;

		/// <summary>
		/// Deep-Copy Assignment Operator
		/// </summary>
		/// <param name="">Reference to Datum to copy from</param>
		/// <returns>Reference to new this Datum</returns>
		Datum& operator=(const Datum&);

		/// <summary>
		/// Move Assignment Operator
		/// </summary>
		/// <param name="">Reference to Datum to move from</param>
		/// <returns>Reference to new this Datum</returns>
		Datum& operator=(Datum&&) noexcept;

		/// <summary>
		/// Typecast Constructor for int32_t Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <param name="capacity">capacity to reserve</param>
		Datum(const std::int32_t, size_type capacity = 1);

		/// <summary>
		/// Typecast Constructor for float Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <param name="capacity">capacity to reserve</param>
		Datum(const float, size_type capacity = 1);

		/// <summary>
		/// Typecast Constructor for vec4 Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <param name="capacity">capacity to reserve</param>
		Datum(const glm::vec4&, size_type capacity = 1);

		/// <summary>
		/// Typecast Constructor for mat4 Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <param name="capacity">capacity to reserve</param>
		Datum(const glm::mat4&, size_type capacity = 1);

		/// <summary>
		/// Typecast Constructor for string Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <param name="capacity">capacity to reserve</param>
		Datum(const std::string&, size_type capacity = 1);

		/// <summary>
		/// Typecast Constructor for Moving string Datum
		/// </summary>
		/// <param name="">data (move) to initialize datum with</param>
		/// <param name="capacity">capacity to reserve</param>
		Datum(std::string&&, size_type capacity = 1);

		/// <summary>
		/// Typecast Constructor for rtti* Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <param name="capacity">capacity to reserve</param>
		Datum(RTTI*, size_type capacity = 1);

		/// <summary>
		/// Typecast Assignment for int32_t Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <returns>Reference to new this Datum</returns>
		Datum& operator=(const std::int32_t);

		/// <summary>
		/// Typecast Assignment for float Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <returns>Reference to new this Datum</returns>
		Datum& operator=(const float);

		/// <summary>
		/// Typecast Assignment for vec4 Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <returns>Reference to new this Datum</returns>
		Datum& operator=(const glm::vec4&);

		/// <summary>
		/// Typecast Assignment for mat4 Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <returns>Reference to new this Datum</returns>
		Datum& operator=(const glm::mat4&);

		/// <summary>
		/// Typecast Assignment for string Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <returns>Reference to new this Datum</returns>
		Datum& operator=(const std::string&);

		/// <summary>
		/// Typecast Assignment for string move Datum
		/// </summary>
		/// <param name="">data (move) to initialize datum with</param>
		/// <returns>Reference to new this Datum</returns>
		Datum& operator=(std::string&&);

		/// <summary>
		/// Typecast Assignment for rtti* Datum
		/// </summary>
		/// <param name="">data to initialize datum with</param>
		/// <returns>Reference to new this Datum</returns>
		Datum& operator=(RTTI*);

		/// <summary>
		/// Equality Operator for another Datum
		/// </summary>
		/// <param name="">other datum to compare with</param>
		/// <returns>TRUE if both same | FALSE if not</returns>
		bool operator==(const Datum&) const;

		/// <summary>
		/// Equality Operator for Datum with in32_t
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both same | FALSE if not</returns>
		bool operator==(const std::int32_t) const;

		/// <summary>
		/// Equality Operator for Datum with float
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both same | FALSE if not</returns>
		bool operator==(const float) const;

		/// <summary>
		/// Equality Operator for Datum with vec4
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both same | FALSE if not</returns>
		bool operator==(const glm::vec4&) const;

		/// <summary>
		/// Equality Operator for Datum with mat4
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both same | FALSE if not</returns>
		bool operator==(const glm::mat4&) const;

		/// <summary>
		/// Equality Operator for Datum with string
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both same | FALSE if not</returns>
		bool operator==(const std::string&) const;

		/// <summary>
		/// Equality Operator for Datum with rtti*
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both same | FALSE if not</returns>
		bool operator==(RTTI*) const;

		/// <summary>
		/// Equality Operator for Datum with scope*
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both same | FALSE if not</returns>
		bool operator==(Scope*) const;

		/// <summary>
		/// NOT-Equality Operator for another Datum
		/// </summary>
		/// <param name="">other datum to compare with</param>
		/// <returns>TRUE if both are different | FALSE if same</returns>
		bool operator!=(const Datum&) const;

		/// <summary>
		/// NOT-Equality Operator for Datum with in32_t
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both are different | FALSE if same</returns>
		bool operator!=(const std::int32_t) const;

		/// <summary>
		/// NOT-Equality Operator for Datum with float
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both are different | FALSE if same</returns>
		bool operator!=(const float) const;

		/// <summary>
		/// NOT-Equality Operator for Datum with vec4
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both are different | FALSE if same</returns>
		bool operator!=(const glm::vec4&) const;

		/// <summary>
		/// NOT-Equality Operator for Datum with mat4
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both are different | FALSE if same</returns>
		bool operator!=(const glm::mat4&) const;

		/// <summary>
		/// NOT-Equality Operator for Datum with string
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both are different | FALSE if same</returns>
		bool operator!=(const std::string&) const;

		/// <summary>
		/// NOT-Equality Operator for Datum with rtti*
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both are different | FALSE if same</returns>
		bool operator!=(RTTI*) const;

		/// <summary>
		/// NOT-Equality Operator for Datum with scope*
		/// </summary>
		/// <param name="">data to compare with</param>
		/// <returns>TRUE if both are different | FALSE if same</returns>
		bool operator!=(Scope*) const;

		/// <summary>
		/// Type of this datum
		/// </summary>
		/// <returns>DatumTypes type</returns>
		DatumTypes Type() const;

		/// <summary>
		/// Change the type of this datum
		/// </summary>
		/// <param name="type">new type</param>
		/// <returns>TRUE if type succesfully changed | FALSE if failed to change</returns>
		bool SetType(DatumTypes type);

		/// <summary>
		/// Is this Datum refering to External Storage
		/// </summary>
		/// <returns>TRUE if refers to external | FALSE if internal</returns>
		bool IsExternalStorage() const;

		/// <summary>
		/// No. of elements in datum
		/// </summary>
		/// <returns>count of elements</returns>
		size_type Size() const;

		/// <summary>
		/// Memory Allocated for this datum
		/// </summary>
		/// <returns>count of memory allocated</returns>
		size_type Capacity() const;

		/// <summary>
		/// Reserve memory
		/// </summary>
		/// <param name="capacity">capacity to reserve</param>
		void Reserve(size_type capacity);

		/// <summary>
		/// Reserve and Construct elements upto given capacity
		/// </summary>
		/// <param name="capacity">capacity to fill datum with</param>
		void ReSize(size_type capacity);

		/// <summary>
		/// Clear all elements from datum
		/// </summary>
		void Clear();

		/// <summary>
		/// Shrink capacity to current size
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Set given index to given data
		/// </summary>
		/// <param name="">int32_t data</param>
		/// <param name="index">index to store at</param>
		void Set(const std::int32_t, size_type index = 0);

		/// <summary>
		/// Set given index to given data
		/// </summary>
		/// <param name="">float data</param>
		/// <param name="index">index to store at</param>
		void Set(const float, size_type index = 0);

		/// <summary>
		/// Set given index to given data
		/// </summary>
		/// <param name="">vec4 data</param>
		/// <param name="index">index to store at</param>
		void Set(const glm::vec4&, size_type index = 0);

		/// <summary>
		/// Set given index to given data
		/// </summary>
		/// <param name="">mat4 data</param>
		/// <param name="index">index to store at</param>
		void Set(const glm::mat4&, size_type index = 0);

		/// <summary>
		/// Set given index to given data
		/// </summary>
		/// <param name="">string data</param>
		/// <param name="index">index to store at</param>
		void Set(const std::string&, size_type index = 0);

		/// <summary>
		/// Set given index to given data
		/// </summary>
		/// <param name="">rtti* data</param>
		/// <param name="index">index to store at</param>
		void Set(RTTI*, size_type index = 0);

		/// <summary>
		/// Push back new data from string
		/// </summary>
		/// <param name="">string containing data</param>
		/// <returns>TRUE if pushback successful | FALSE if failed</returns>
		bool PushBackFromString(const std::string&);

		/// <summary>
		/// Set data at index using string
		/// </summary>
		/// <param name="">string containing data</param>
		/// <param name="index">index to set</param>
		/// <returns>TRUE if pushback successful | FALSE if failed</returns>
		bool SetFromString(const std::string&, size_type index = 0);

		/// <summary>
		/// Convert data to String at given index
		/// </summary>
		/// <param name="index">index of data</param>
		/// <returns>string-ified data</returns>
		std::string ToString(size_type index = 0) const;

		/// <summary>
		/// Get data at given index
		/// </summary>
		/// <typeparam name="T">type of data</typeparam>
		/// <param name="index">index of data</param>
		/// <returns>reference to data</returns>
		template<typename T>
		T& Get(size_type index = 0);

		/// <summary>
		/// Get const data at given index
		/// </summary>
		/// <typeparam name="T">type of data</typeparam>
		/// <param name="index">index of data</param>
		/// <returns>reference to const data</returns>
		template<typename T>
		const T& Get(size_type index = 0) const;

		/// <summary>
		/// [] Operator overload to dereference scope
		/// </summary>
		/// <param name="index">index of scope pointer</param>
		/// <returns>reference to scope</returns>
		Scope& operator[](size_type index);

		/// <summary>
		/// [] Operator overload to dereference scope
		/// </summary>
		/// <param name="index">index of scope pointer</param>
		/// <returns>reference to scope</returns>
		const Scope& operator[](size_type index) const;

		/// <summary>
		/// Get data at Front of Datum
		/// </summary>
		/// <returns>reference to data</returns>
		template<typename T>
		T& Front();

		/// <summary>
		/// Get const data at Front of Datum
		/// </summary>
		/// <returns>reference to const data</returns>
		template<typename T>
		const T& Front() const;

		/// <summary>
		/// Get data at Back of Datum
		/// </summary>
		/// <returns>reference to data</returns>
		template<typename T>
		T& Back();

		/// <summary>
		/// Get const data at Back of Datum
		/// </summary>
		/// <returns>reference to const data</returns>
		template<typename T>
		const T& Back() const;

		/// <summary>
		/// PushBack int32_t data into datum
		/// </summary>
		/// <param name="">data to push</param>
		void PushBack(const std::int32_t);

		/// <summary>
		/// PushBack float data into datum
		/// </summary>
		/// <param name="">data to push</param>
		void PushBack(const float);

		/// <summary>
		/// PushBack vec4 data into datum
		/// </summary>
		/// <param name="">data to push</param>
		void PushBack(const glm::vec4&);

		/// <summary>
		/// PushBack mat4 data into datum
		/// </summary>
		/// <param name="">data to push</param>
		void PushBack(const glm::mat4&);

		/// <summary>
		/// PushBack string data into datum
		/// </summary>
		/// <param name="">data to push</param>
		void PushBack(const std::string&);

		/// <summary>
		/// PushBack MOVE string data into datum
		/// </summary>
		/// <param name="">data to push</param>
		void PushBack(std::string&&);

		/// <summary>
		/// PushBack rtti* data into datum
		/// </summary>
		/// <param name="">data to push</param>
		void PushBack(RTTI*);

		/// <summary>
		/// Pop last element from Datum
		/// </summary>
		void PopBack();

		/// <summary>
		/// Find where passed data exists in datum
		/// </summary>
		/// <param name="">int32_t data</param>
		/// <returns>index if found | current size of datum if not found</returns>
		size_type Find(const std::int32_t) const;

		/// <summary>
		/// Find where passed data exists in datum
		/// </summary>
		/// <param name="">float data</param>
		/// <returns>index if found | current size of datum if not found</returns>
		size_type Find(const float) const;

		/// <summary>
		/// Find where passed data exists in datum
		/// </summary>
		/// <param name="">vec4 data</param>
		/// <returns>index if found | current size of datum if not found</returns>
		size_type Find(const glm::vec4&) const;

		/// <summary>
		/// Find where passed data exists in datum
		/// </summary>
		/// <param name="">mat4 data</param>
		/// <returns>index if found | current size of datum if not found</returns>
		size_type Find(const glm::mat4&) const;

		/// <summary>
		/// Find where passed data exists in datum
		/// </summary>
		/// <param name="">string data</param>
		/// <returns>index if found | current size of datum if not found</returns>
		size_type Find(const std::string&) const;

		/// <summary>
		/// Find where passed data exists in datum
		/// </summary>
		/// <param name="">rtti* data</param>
		/// <returns>index if found | current size of datum if not found</returns>
		size_type Find(RTTI*) const;

		/// <summary>
		/// Find where passed data exists in datum
		/// </summary>
		/// <param name="">scope* data</param>
		/// <returns>index if found | current size of datum if not found</returns>
		size_type Find(Scope&);

		/// <summary>
		/// const Find where passed data exists in datum
		/// </summary>
		/// <param name="">scope* data</param>
		/// <returns>index if found | current size of datum if not found</returns>
		size_type Find(const Scope&) const;

		/// <summary>
		/// Remove passed data (if exists) from Datum
		/// </summary>
		/// <param name="">int32_t data to delete</param>
		/// <returns>TRUE if something was deleted | FALSE if nothing was deleted</returns>
		bool Remove(const std::int32_t);

		/// <summary>
		/// Remove passed data (if exists) from Datum
		/// </summary>
		/// <param name="">float data to delete</param>
		/// <returns>TRUE if something was deleted | FALSE if nothing was deleted</returns>
		bool Remove(const float);

		/// <summary>
		/// Remove passed data (if exists) from Datum
		/// </summary>
		/// <param name="">vec4 data to delete</param>
		/// <returns>TRUE if something was deleted | FALSE if nothing was deleted</returns>
		bool Remove(const glm::vec4&);

		/// <summary>
		/// Remove passed data (if exists) from Datum
		/// </summary>
		/// <param name="">mat4 data to delete</param>
		/// <returns>TRUE if something was deleted | FALSE if nothing was deleted</returns>
		bool Remove(const glm::mat4&);

		/// <summary>
		/// Remove passed data (if exists) from Datum
		/// </summary>
		/// <param name="">string data to delete</param>
		/// <returns>TRUE if something was deleted | FALSE if nothing was deleted</returns>
		bool Remove(const std::string&);

		/// <summary>
		/// Remove passed data (if exists) from Datum
		/// </summary>
		/// <param name="">rtti* data to delete</param>
		/// <returns>TRUE if something was deleted | FALSE if nothing was deleted</returns>
		bool Remove(RTTI*);

		/// <summary>
		/// Remove data at given index
		/// </summary>
		/// <param name="index">index to delete</param>
		/// <returns>TRUE if something was deleted | FALSE if nothing was deleted</returns>
		bool RemoveAt(size_type index);

		/// <summary>
		/// Set datum to point to external int32_t storage
		/// </summary>
		/// <param name="">pointer to external storage</param>
		/// <param name="">no. of elements in external storage</param>
		void SetStorageExternal(std::int32_t* const, size_type);

		/// <summary>
		/// Set datum to point to external float storage
		/// </summary>
		/// <param name="">pointer to external storage</param>
		/// <param name="">no. of elements in external storage</param>
		void SetStorageExternal(float* const, size_type);

		/// <summary>
		/// Set datum to point to external vec4 storage
		/// </summary>
		/// <param name="">pointer to external storage</param>
		/// <param name="">no. of elements in external storage</param>
		void SetStorageExternal(glm::vec4* const, size_type);

		/// <summary>
		/// Set datum to point to external mat4 storage
		/// </summary>
		/// <param name="">pointer to external storage</param>
		/// <param name="">no. of elements in external storage</param>
		void SetStorageExternal(glm::mat4* const, size_type);

		/// <summary>
		/// Set datum to point to external string storage
		/// </summary>
		/// <param name="">pointer to external storage</param>
		/// <param name="">no. of elements in external storage</param>
		void SetStorageExternal(std::string* const, size_type);

		/// <summary>
		/// Set datum to point to external rtti* storage
		/// </summary>
		/// <param name="">pointer to external storage</param>
		/// <param name="">no. of elements in external storage</param>
		void SetStorageExternal(RTTI** const, size_type);

		/// <summary>
		/// Change Increment Functor for increasing capacity of datum
		/// </summary>
		/// <param name="">std::function returning size_t and paramters(size_t size, size_t capacity)</param>
		void SetIncrementFunctor(std::function<size_type(size_type, size_type)>);

	private:
		void SetStorageExternal(void* const, DatumTypes, size_type);

		Datum(Scope&, size_type capacity = 1);
		void PushBack(Scope&);
		void Set(Scope&, size_type index = 0);

	private:
		DatumVals _data;
		DatumTypes _type{ DatumTypes::Unknown };
		size_type _capacity{ 0 };
		size_type _size{ 0 };

		bool _isExternalStorage{ false };

		std::function<size_type(size_type, size_type)> _incrementFunc{ DefaultIncrementFunct{} };

	public:
		static const size_type DatumTypeSizes[static_cast<size_type>(DatumTypes::End) + 1];
		static const HashMap<std::string, DatumTypes> DatumStringToType;
	};
}

#include "Datum.inl"
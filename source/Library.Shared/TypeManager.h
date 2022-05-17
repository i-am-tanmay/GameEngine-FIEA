#pragma once
#include <cstdint>
#include <stdexcept>
#include "AttributeSignature.h"
#include "HashMap.h"

namespace FIEAGameEngine
{
	class TypeManager final
	{
	public:
		using size_type = std::size_t;
		using _HashMapSignature = HashMap<size_type, Vector<AttributeSignature>>;
		using _TPair = typename _HashMapSignature::_TPair;

#pragma region SINGLETON
	public:
		/// <summary>
		/// Get Singleton Instance of TypeManager
		/// </summary>
		/// <returns>Current instance of typemanager</returns>
		static TypeManager& GetInstance();

		TypeManager(const TypeManager&) = delete;
		TypeManager(TypeManager&&) = delete;
		TypeManager& operator=(const TypeManager&) = delete;
		TypeManager& operator=(TypeManager&&) = delete;

	private:
		TypeManager() {};
#pragma endregion

	public:
		/// <summary>
		/// Register new class into the Types List
		/// </summary>
		/// <param name="type">TypeID of class to register</param>
		/// <param name="signature">Vector of signatures of class</param>
		/// <param name="signature_parent">Vector of signatures of parent class</param>
		void RegisterClass(size_type type, Vector<AttributeSignature>&& signature, size_type parent_id);

		/// <summary>
		/// Register new class into the Types List
		/// </summary>
		/// <param name="type">TypeID of class to register</param>
		/// <param name="signature">Vector of signatures of class</param>
		void RegisterClass(size_type type, Vector<AttributeSignature>&& signature);

		/// <summary>
		/// Get List of Prescribed Attributes of a Class
		/// </summary>
		/// <param name="type">TypeID of class to get attributes of</param>
		/// <returns>Const Vector of Prescribed Attribute Signatures</returns>
		const Vector<AttributeSignature>& GetPrescribedAttributes(size_type type) const;

		/// <summary>
		/// Clear the Map of stored Signature Lists
		/// </summary>
		void Clear();

	private:
		_HashMapSignature _map;

	};

#define RegisterType(Type, ParentType) TypeManager::GetInstance().RegisterClass(Type::TypeIdClass(), std::move(Type::Signatures()), ParentType::TypeIdClass());
#define RegisterSingleType(Type) TypeManager::GetInstance().RegisterClass(Type::TypeIdClass(), std::move(Type::Signatures()));
}
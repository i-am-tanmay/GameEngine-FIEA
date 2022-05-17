#include "pch.h"
#include "TypeManager.h"

namespace FIEAGameEngine
{
	TypeManager& TypeManager::GetInstance()
	{
		static TypeManager _instance;
		return _instance;
	}

	void TypeManager::RegisterClass(size_type type, Vector<AttributeSignature>&& signature)
	{
		_map.Insert(std::move(_TPair(type, std::move(signature))));
	}

	void TypeManager::RegisterClass(size_type type, Vector<AttributeSignature>&& signature, size_type parent_id)
	{
		Vector<AttributeSignature> signature_parent = _map.At(parent_id);
		_map.Insert(std::move(_TPair(type, std::move(signature_parent + std::move(signature)))));
	}

	const Vector<AttributeSignature>& TypeManager::GetPrescribedAttributes(size_type type) const
	{
		return _map.At(type);
	}

	void TypeManager::Clear()
	{
		_map.Clear();
	}
}
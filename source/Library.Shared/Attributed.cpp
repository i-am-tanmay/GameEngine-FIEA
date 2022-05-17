#include "pch.h"
#include "Attributed.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Attributed, Scope);

	Vector<AttributeSignature> Attributed::Signatures() { return Vector<AttributeSignature>{}; }

	Attributed::Attributed(RTTI::IdType type)
	{
		Append("this"s, DatumTypes::Pointer) = this;

		const Vector<AttributeSignature>& prescribedattr = TypeManager::GetInstance().GetPrescribedAttributes(type);
		for (const AttributeSignature& signature : prescribedattr)
		{
			Datum& datum = Append(signature._name, signature._type);
			if (datum.Type() != signature._type) throw std::runtime_error("Duplicate Entry of different type");
			if (signature._offset > 0) datum.SetStorageExternal(reinterpret_cast<std::uint8_t*>(this) + signature._offset, signature._type, signature._size);
		}

		_prescribedCount = _orderTable.Size();
	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs), _prescribedCount{ rhs._prescribedCount }
	{
		UpdateExternalStorage(reinterpret_cast<const std::uint8_t* const>(&rhs));
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);
			_prescribedCount = rhs._prescribedCount;
			UpdateExternalStorage(reinterpret_cast<const std::uint8_t* const>(&rhs));
		}

		return *this;
	}

	Attributed::Attributed(Attributed&& rhs) noexcept : Scope(std::move(rhs)), _prescribedCount{ rhs._prescribedCount }
	{
		UpdateExternalStorage(reinterpret_cast<const std::uint8_t* const>(&rhs));
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Scope::operator=(std::move(rhs));
			_prescribedCount = rhs._prescribedCount;
			UpdateExternalStorage(reinterpret_cast<const std::uint8_t* const>(&rhs));
		}

		return *this;
	}

	void Attributed::UpdateExternalStorage(const uint8_t* const rhsptr)
	{
		_orderTable[0]->second.Set(this);

		std::uint8_t* const thisptr = reinterpret_cast<std::uint8_t* const>(this);
		for (size_type i = 1; i < _prescribedCount; ++i)
		{
			auto& [key, datum] = *(_orderTable[i]);
			if (datum._isExternalStorage) datum._data._voidp = thisptr + (datum._data._byte - rhsptr);
		}
	}

	bool Attributed::IsAttribute(const std::string& key) const
	{
		return (_hashmap.Find(key) != _hashmap.cend());
	}

	bool Attributed::IsPrescribedAttribute(const std::string& key) const
	{
		for (size_type i = 0; i < _prescribedCount; ++i)
		{
			if (std::strcmp(key.c_str(), _orderTable[i]->first.c_str()) == 0)
			{
				return true;
			}
		}

		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& key) const
	{
		return (IsAttribute(key) && !IsPrescribedAttribute(key));
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& key)
	{
		if (IsPrescribedAttribute(key)) throw std::runtime_error("Entry is a prescribed Attribute");
		return Append(key);
	}
}
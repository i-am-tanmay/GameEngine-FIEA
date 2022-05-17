#include "pch.h"
#include "AttributedFoo.h"
#include "Datum.h"

using namespace std::string_literals;

using namespace FIEAGameEngine;
namespace LibraryDesktopTests
{
	RTTI_DEFINITIONS(AttributedFoo, FIEAGameEngine::Attributed);

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{ }

	AttributedFoo::AttributedFoo(size_type _typeid) :
		Attributed(_typeid), ExternalVector{}, ExternalMatrix{}, ExternalIntegerArray{}, ExternalFloatArray{}, ExternalVectorArray{}, ExternalMatrixArray{}
	{ }

	Vector<AttributeSignature> AttributedFoo::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{"ExternalInteger", DatumTypes::Integer, 1u, offsetof(AttributedFoo, ExternalInteger)},
			{ "ExternalFloat", DatumTypes::Float, 1u, offsetof(AttributedFoo, ExternalFloat) },
			{ "ExternalString", DatumTypes::String, 1u, offsetof(AttributedFoo, ExternalString) },
			{ "ExternalVector", DatumTypes::Vector, 1u, offsetof(AttributedFoo, ExternalVector) },
			{ "ExternalMatrix", DatumTypes::Matrix, 1u, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray", DatumTypes::Integer, ArraySize, offsetof(AttributedFoo, ExternalIntegerArray) },
			{ "ExternalFloatArray", DatumTypes::Float, ArraySize, offsetof(AttributedFoo, ExternalFloatArray) },
			{ "ExternalStringArray", DatumTypes::String, ArraySize, offsetof(AttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray", DatumTypes::Vector, ArraySize, offsetof(AttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray", DatumTypes::Matrix, ArraySize, offsetof(AttributedFoo, ExternalMatrixArray) },
			{ "NestedScope", DatumTypes::ScopeTable, 0u, 0u },
			{ "NestedScopeArray", DatumTypes::ScopeTable, ArraySize, 0u }
		};
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		const AttributedFoo* const rhsFoo = rhs->As<AttributedFoo>();
		if (rhsFoo == nullptr) return false;

		return ExternalInteger == rhsFoo->ExternalInteger &&
			ExternalFloat == rhsFoo->ExternalFloat &&
			ExternalString == rhsFoo->ExternalString &&
			ExternalVector == rhsFoo->ExternalVector &&
			ExternalMatrix == rhsFoo->ExternalMatrix &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalFloatArray), std::end(ExternalFloatArray), std::begin(rhsFoo->ExternalFloatArray)) &&
			std::equal(std::begin(ExternalStringArray), std::end(ExternalStringArray), std::begin(rhsFoo->ExternalStringArray)) &&
			std::equal(std::begin(ExternalVectorArray), std::end(ExternalVectorArray), std::begin(rhsFoo->ExternalVectorArray)) &&
			std::equal(std::begin(ExternalMatrixArray), std::end(ExternalMatrixArray), std::begin(rhsFoo->ExternalMatrixArray)) &&
			At("NestedScope"s) == rhsFoo->At("NestedScope"s);
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo"s;
	}
}
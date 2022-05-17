#include "pch.h"
#include "AbsolutePerfectionAttributedFoo.h"
#include "Datum.h"

using namespace std::string_literals;

using namespace FIEAGameEngine;
namespace LibraryDesktopTests
{
	RTTI_DEFINITIONS(AbsolutePerfectionAttributedFoo, AttributedFoo);

	AbsolutePerfectionAttributedFoo::AbsolutePerfectionAttributedFoo() :
		AttributedFoo(AbsolutePerfectionAttributedFoo::TypeIdClass())
	{ }

	Vector<AttributeSignature> AbsolutePerfectionAttributedFoo::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{"ExternalInteger", DatumTypes::Integer, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalInteger)},
			{ "ExternalFloat", DatumTypes::Float, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalFloat) },
			{ "ExternalStringPerfection", DatumTypes::String, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalStringPerfection) },
			{ "ExternalVectorPerfection", DatumTypes::Vector, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalVectorPerfection) },
			{ "ExternalMatrixPerfection", DatumTypes::Matrix, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArrayPerfection", DatumTypes::Integer, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalIntegerArrayPerfection) },
			{ "ExternalFloatArrayPerfection", DatumTypes::Float, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalFloatArrayPerfection) },
			{ "ExternalStringArray", DatumTypes::String, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray", DatumTypes::Vector, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray", DatumTypes::Matrix, 1u, offsetof(AbsolutePerfectionAttributedFoo, ExternalMatrixArray) },
			{ "NestedScope", DatumTypes::ScopeTable, 0u, 0u },
			{ "NestedScopeArrayPerfection", DatumTypes::ScopeTable, ArraySize, 0u }
		};
	}

	gsl::owner<AbsolutePerfectionAttributedFoo*> AbsolutePerfectionAttributedFoo::Clone() const
	{
		return new AbsolutePerfectionAttributedFoo(*this);
	}

	bool AbsolutePerfectionAttributedFoo::Equals(const RTTI* rhs) const
	{
		const AbsolutePerfectionAttributedFoo* const rhsFoo = rhs->As<AbsolutePerfectionAttributedFoo>();
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

	std::string AbsolutePerfectionAttributedFoo::ToString() const
	{
		return "AbsolutePerfectionAttributedFoo"s;
	}
}

namespace LibraryDesktopTests
{
	RTTI_DEFINITIONS(AbsoluteGarbageAttributedFoo, AttributedFoo);

	AbsoluteGarbageAttributedFoo::AbsoluteGarbageAttributedFoo() :
		AttributedFoo(AbsoluteGarbageAttributedFoo::TypeIdClass())
	{ }

	Vector<AttributeSignature> AbsoluteGarbageAttributedFoo::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{"ExternalInteger", DatumTypes::Float, 1u, offsetof(AbsoluteGarbageAttributedFoo, ExternalInteger)}
		};
	}
}
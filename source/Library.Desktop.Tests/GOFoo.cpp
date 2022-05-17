#include "pch.h"
#include "GOFoo.h"
#include "Datum.h"

using namespace std::string_literals;

namespace LibraryDesktopTests
{
	RTTI_DEFINITIONS(GOFoo, GameObject);

	GOFoo::GOFoo() :
		GameObject(GOFoo::TypeIdClass())
	{ }

	Vector<AttributeSignature> GOFoo::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{"testfloat", DatumTypes::Float, 1u, offsetof(GOFoo, testfloat)}
		};
	}

	void GOFoo::Update(const GameTime& gametime)
	{
		testfloat += .5f;
		GameObject::Update(gametime);
	}

	gsl::owner<GOFoo*> GOFoo::Clone() const
	{
		return new GOFoo(*this);
	}

	bool GOFoo::Equals(const RTTI* rhs) const
	{
		const GOFoo* const rhsFoo = rhs->As<GOFoo>();
		if (rhsFoo == nullptr) return false;

		return testfloat == rhsFoo->testfloat;
	}

	std::string GOFoo::ToString() const
	{
		return Name;
	}
}
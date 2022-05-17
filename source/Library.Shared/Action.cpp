#include "pch.h"
#include "Action.h"

using namespace std::string_literals;
namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Action, Attributed);

	Action::Action(size_type _typeid) :
		Attributed(_typeid)
	{ }

	Vector<AttributeSignature> Action::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{ "Name", DatumTypes::String, 1u, offsetof(Action, Name) }
		};
	}
}
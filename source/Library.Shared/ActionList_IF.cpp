#include "pch.h"
#include "ActionList_IF.h"

using namespace std::string_literals;
namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionList_IF, ActionList);

	ActionList_IF::ActionList_IF() :
		ActionList(ActionList_IF::TypeIdClass())
	{ }

	Vector<AttributeSignature> ActionList_IF::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{ "condition", DatumTypes::Integer, 1u, offsetof(ActionList_IF, _condition) }
		};
	}

	void ActionList_IF::Update(const GameTime& gametime)
	{
		if (_condition > 1) _condition = 1;

		_actions[_condition].get().Update(gametime);
	}

	gsl::owner<ActionList_IF*> ActionList_IF::Clone() const
	{
		return new ActionList_IF(*this);
	}

	bool ActionList_IF::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) return false;
		const ActionList_IF* const rhsgo = rhs->As<ActionList_IF>();
		if (rhsgo == nullptr) return false;

		return this == rhsgo;
	}

	std::string ActionList_IF::ToString() const
	{
		return Name;
	}
}
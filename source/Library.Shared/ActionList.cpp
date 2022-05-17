#include "pch.h"
#include "ActionList.h"

using namespace std::string_literals;
namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionList, Action);

	ActionList::ActionList() :
		Action(ActionList::TypeIdClass())
	{ }

	ActionList::ActionList(size_type _typeid) :
		Action(_typeid)
	{ }

	Vector<AttributeSignature> ActionList::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{ "Actions", DatumTypes::ScopeTable, 0u, 0u }
		};
	}

	void ActionList::AddAction(Action& action)
	{
		for (size_type i = 0; i < _actions.Size(); ++i) if (&(_actions[i].get()) == (&action)) return;

		Adopt(action, "Actions");
		_actions.PushBack(action);
	}

	void ActionList::RemoveAction(Action& action)
	{
		for (Vector<std::reference_wrapper<Action>>::Iterator itr = _actions.begin(); itr != _actions.end(); ++itr)
		{
			if (&((*itr).get()) == &action)
			{
				_actions.Remove(itr);
				action.RemoveParent();
				return;
			}
		}
	}

	void ActionList::Update(const GameTime& gametime)
	{
		for (Action& act : _actions) act.Update(gametime);
	}

	gsl::owner<ActionList*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	bool ActionList::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) return false;
		const ActionList* const rhsgo = rhs->As<ActionList>();
		if (rhsgo == nullptr) return false;

		if (_actions.Size() != rhsgo->_actions.Size()) return false;
		for (std::size_t i = 0; i < _actions.Size(); ++i) if (&(_actions[i].get()) != &(rhsgo->_actions[i].get())) return false;

		return true;
	}

	std::string ActionList::ToString() const
	{
		return Name;
	}
}
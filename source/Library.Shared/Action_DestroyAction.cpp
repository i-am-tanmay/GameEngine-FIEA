#include "pch.h"
#include "Action_DestroyAction.h"
#include "ActionManager.h"

using namespace std::string_literals;
namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Action_DestroyAction, Action);

	Action_DestroyAction::Action_DestroyAction() :
		Action(Action_DestroyAction::TypeIdClass())
	{ }

	Vector<AttributeSignature> Action_DestroyAction::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{ "DestroyAction", DatumTypes::String, 1u, offsetof(Action_DestroyAction, DestroyName) }
		};
	}

	void Action_DestroyAction::Update(const GameTime&)
	{
		Scope* context{ this };
		Datum* datum{ nullptr };
		while (context->GetParent() != nullptr)
		{
			context = context->GetParent();

			Action* action = context->As<Action>();
			if (action != nullptr && action->Name == DestroyName)
			{
				ActionManager::DestroyActionTask(*action);
				return;
			}

			datum = context->Find("Actions");
			if (datum != nullptr && datum->Type() == DatumTypes::ScopeTable)
			{
				Action* target;
				for (std::size_t i = 0; i < datum->Size(); ++i)
				{
					target = datum->Get<Scope*>(i)->As<Action>();
					if (target != nullptr && target->Name == DestroyName)
					{
						ActionManager::DestroyActionTask(*target);
						return;
					}
				}
			}

			datum = context->Find("Children");
			if (datum != nullptr && datum->Type() == DatumTypes::ScopeTable)
			{
				for (std::size_t i = 0; i < datum->Size(); ++i)
				{
					GameObject* obj = datum->Get<Scope*>(i)->As<GameObject>();
					if (obj != nullptr)
					{
						Datum* datum_action = obj->Find("Actions");
						if (datum_action != nullptr && datum_action->Type() == DatumTypes::ScopeTable)
						{
							Action* target;
							for (std::size_t ii = 0; ii < datum_action->Size(); ++ii)
							{
								target = datum_action->Get<Scope*>(ii)->As<Action>();
								if (target != nullptr && target->Name == DestroyName)
								{
									ActionManager::DestroyActionTask(*target);
									return;
								}
							}
						}
					}
				}
				
			}
		}
	}

	gsl::owner<Action_DestroyAction*> Action_DestroyAction::Clone() const
	{
		return new Action_DestroyAction(*this);
	}

	bool Action_DestroyAction::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) return false;
		const Action_DestroyAction* const rhsgo = rhs->As<Action_DestroyAction>();
		if (rhsgo == nullptr) return false;

		return this == rhsgo;
	}

	std::string Action_DestroyAction::ToString() const
	{
		return Name;
	}
}
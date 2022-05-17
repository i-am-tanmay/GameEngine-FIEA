#include "pch.h"
#include "Action_CreateAction.h"
#include "ActionList.h"
#include "ActionManager.h"

using namespace std::string_literals;
namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Action_CreateAction, Action);

	Action_CreateAction::Action_CreateAction() :
		Action(Action_CreateAction::TypeIdClass())
	{ }

	Vector<AttributeSignature> Action_CreateAction::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{ "actionclass", DatumTypes::String, 1u, offsetof(Action_CreateAction, Class) },
			{ "CreateName", DatumTypes::String, 1u, offsetof(Action_CreateAction, CreateName) },
			{ "CreateContext", DatumTypes::String, 1u, offsetof(Action_CreateAction, CreateContext) }
		};
	}

	void Action_CreateAction::Update(const GameTime&)
	{
		GameObject* createcontext_go{ nullptr };
		ActionList* createcontext_aclist{ nullptr };

		Scope* context{ this };
		Datum* datum{ nullptr };

		while (context->GetParent() != nullptr)
		{
			context = context->GetParent();

			GameObject* parent_go = context->As<GameObject>();
			if (parent_go != nullptr && parent_go->Name == CreateContext)
			{
				createcontext_go = parent_go;
				break;
			}

			ActionList* parent_aclist = context->As<ActionList>();
			if (parent_aclist != nullptr && parent_aclist->Name == CreateContext)
			{
				createcontext_aclist = parent_aclist;
				break;
			}

			datum = context->Find("Children");
			if (datum != nullptr && datum->Type() == DatumTypes::ScopeTable)
			{
				GameObject* target{ nullptr };
				for (std::size_t i = 0; i < datum->Size(); ++i)
				{
					target = datum->Get<Scope*>(i)->As<GameObject>();
					if (target != nullptr && target->Name == CreateContext)
					{
						createcontext_go = target;
						break;
					}

					Datum* child_datum = target->Find("Actions"s);
					ActionList* target_aclist{ nullptr };
					for (std::size_t ii = 0; ii < child_datum->Size(); ++ii)
					{
						target_aclist = child_datum->Get<Scope*>(ii)->As<ActionList>();
						if (target_aclist != nullptr && target_aclist->Name == CreateContext)
						{
							createcontext_aclist = target_aclist;
							break;
						}
					}
					if (createcontext_aclist != nullptr) break;
				}
			}
			if (createcontext_go != nullptr || createcontext_aclist != nullptr) break;

			datum = context->Find("Actions");
			if (datum != nullptr && datum->Type() == DatumTypes::ScopeTable)
			{
				ActionList* target{ nullptr };
				for (std::size_t i = 0; i < datum->Size(); ++i)
				{
					target = datum->Get<Scope*>(i)->As<ActionList>();
					if (target != nullptr && target->Name == CreateContext)
					{
						createcontext_aclist = target;
						break;
					}
				}
			}
			if (createcontext_aclist != nullptr) break;
		}

		if (createcontext_go == nullptr && createcontext_aclist == nullptr) return;
		Action* action = Factory<Action>::Create(Class);
		if (action == nullptr) return;
		action->Name = CreateName;

		if (createcontext_go != nullptr) ActionManager::CreateActionTask(*createcontext_go, *action);
		else ActionManager::CreateActionTask(*createcontext_aclist, *action);
	}

	gsl::owner<Action_CreateAction*> Action_CreateAction::Clone() const
	{
		return new Action_CreateAction(*this);
	}

	bool Action_CreateAction::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) return false;
		const Action_CreateAction* const rhsgo = rhs->As<Action_CreateAction>();
		if (rhsgo == nullptr) return false;

		return this == rhsgo;
	}

	std::string Action_CreateAction::ToString() const
	{
		return Name;
	}
}
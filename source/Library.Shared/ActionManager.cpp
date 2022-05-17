#include "pch.h"
#include "ActionManager.h"
#include "GameObject.h"
#include "ActionList.h"

namespace FIEAGameEngine
{
	void ActionManager::CreateActionTask(GameObject& parent, Action& child)
	{
		_createQ_go.PushBack(std::pair<GameObject*, Action*>(&parent, &child));
	}

	void ActionManager::CreateActionTask(ActionList& parent, Action& child)
	{
		_createQ_aclist.PushBack(std::pair<ActionList*, Action*>(&parent, &child));
	}

	void ActionManager::DestroyActionTask(Action& action)
	{
		for (Action* d : _destroyQ) if (d == &action) return;
		_destroyQ.PushBack(&action);
	}

	void ActionManager::Update()
	{
		for (std::pair<GameObject*, Action*>& c : _createQ_go)
		{
			if (c.first != nullptr && c.second != nullptr)
			{
				c.first->AddAction(*(c.second));
			}
		}
		_createQ_go.Clear();

		for (std::pair<ActionList*, Action*>& c : _createQ_aclist)
		{
			if (c.first != nullptr && c.second != nullptr)
			{
				c.first->AddAction(*(c.second));
			}
		}
		_createQ_aclist.Clear();

		for (Action* d : _destroyQ)
		{
			if (d != nullptr)
			{
				Scope* parent{ d->GetParent() };
				GameObject* go = parent->As<GameObject>();
				if (go != nullptr) go->RemoveAction(*d);
				else
				{
					ActionList* acl = parent->As<ActionList>();
					acl->RemoveAction(*d);
				}

				delete d;
			}
		}
		_destroyQ.Clear();
	}

	void ActionManager::CleanUp()
	{
		_createQ_go.Clear();
		_createQ_go.ShrinkToFit();

		_createQ_aclist.Clear();
		_createQ_aclist.ShrinkToFit();

		_destroyQ.Clear();
		_destroyQ.ShrinkToFit();
	}
}
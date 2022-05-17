#include "pch.h"
#include "GameObject.h"

using namespace std::string_literals;
namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(GameObject, Attributed);
	RTTI_DEFINITIONS(Transform, Attributed);

	GameObject::GameObject() :
		Attributed(GameObject::TypeIdClass())
	{ }

	GameObject::GameObject(size_type _typeid) :
		Attributed(_typeid)
	{ }

	Vector<AttributeSignature> GameObject::Signatures()
	{
		return Vector<AttributeSignature>
		{
			{ "Name", DatumTypes::String, 1u, offsetof(GameObject, Name) },
			{ "Transform", DatumTypes::ScopeTable, 1u, offsetof(GameObject, transform) },
			{ "Children", DatumTypes::ScopeTable, 0u, 0u },
			{ "Actions", DatumTypes::ScopeTable, 0u, 0u }
		};
	}

	void GameObject::AddChild(GameObject& child)
	{
		for (size_type i = 0; i < _children.Size(); ++i) if (&(_children[i].get()) == &child) return;
		
		Adopt(child, "Children");
		_children.PushBack(child);
	}

	void GameObject::AddAction(Action& action)
	{
		for (size_type i = 0; i < _actions.Size(); ++i) if (&(_actions[i].get()) == (&action)) return;

		Adopt(action, "Actions");
		_actions.PushBack(action);
	}

	void GameObject::RemoveAction(Action& action)
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

	void GameObject::Update(const GameTime& gametime)
	{
		for (GameObject& go : _children) go.Update(gametime);
		for (Action& act : _actions) act.Update(gametime);
	}

	gsl::owner<GameObject*> GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	bool GameObject::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) return false;
		const GameObject* const rhsgo = rhs->As<GameObject>();
		if (rhsgo == nullptr) return false;

		return Name == rhsgo->Name
			&& transform->At("Position") == rhsgo->transform->At("Position")
			&& transform->At("Rotation") == rhsgo->transform->At("Rotation")
			&& transform->At("Scale") == rhsgo->transform->At("Scale")
			&& At("Children"s) == rhsgo->At("Children"s);
	}

	std::string GameObject::ToString() const
	{
		return Name;
	}
}
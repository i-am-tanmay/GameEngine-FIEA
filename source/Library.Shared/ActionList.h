#pragma once
#include "GameTime.h"
#include "Action.h"
#include "GameObject.h"

namespace FIEAGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList);

	public:
		/// <summary>
		/// Constructor for ActionList
		/// </summary>
		ActionList();

		/// <summary>
		/// Adopt Action into this List
		/// </summary>
		/// <param name="action"></param>
		void AddAction(Action& action);

		/// <summary>
		/// Remove an action derived Action from this Action List
		/// </summary>
		/// <param name="action">Reference to action to remove</param>
		void RemoveAction(Action& action);

		/// <summary>
		/// Update all Actions in this List
		/// </summary>
		/// <param name="gametime">Reference to current gametime</param>
		virtual void Update(const GameTime& gametime) override;

		/// <summary>
		/// Clone this action list
		/// </summary>
		/// <returns>Raw Pointer to cloned action list</returns>
		virtual gsl::owner<ActionList*> Clone() const override;

		/// <summary>
		/// Equality for action lists
		/// </summary>
		/// <param name="rhs">Pointer to other action list</param>
		/// <returns>TRUE if both action lists have same actions and in same order | FALSE if not</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Name of this ActionList
		/// </summary>
		/// <returns></returns>
		std::string ToString() const override;

		/// <summary>
		/// Get Prescribed Attributes for ActionList Class
		/// </summary>
		/// <returns>Vector of Prescribed Attribute Signatures</returns>
		static Vector<AttributeSignature> Signatures();

	protected:
		ActionList(size_type _typeid);

		Vector<std::reference_wrapper<Action>> _actions;
	};

	DefineFactory(ActionList, Action);
}
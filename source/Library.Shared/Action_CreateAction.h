#pragma once
#include "GameTime.h"
#include "Action.h"
#include "GameObject.h"

namespace FIEAGameEngine
{
	class ActionList;
	class Action_CreateAction final : public Action
	{
		RTTI_DECLARATIONS(Action_CreateAction);

	public:
		/// <summary>
		/// Constructor: for Action - Create Action
		/// </summary>
		Action_CreateAction();

		/// <summary>
		/// Create a new Action from given class and name into given context, add a task to adopt to Action Manager
		/// </summary>
		/// <param name="gametime">Reference to current gametime</param>
		void Update(const GameTime& gametime) override;

		/// <summary>
		/// Clone this Create action
		/// </summary>
		/// <returns>Raw Pointer to cloned action</returns>
		virtual gsl::owner<Action_CreateAction*> Clone() const override;

		/// <summary>
		/// Equality for CreateAction action
		/// </summary>
		/// <param name="rhs">Pointer to other action</param>
		/// <returns>TRUE if both actions refer to same action | FALSE if not</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Name of this action
		/// </summary>
		/// <returns></returns>
		std::string ToString() const override;

		/// <summary>
		/// Get Prescribed Attributes for Action CreateAction Class
		/// </summary>
		/// <returns>Vector of Prescribed Attribute Signatures</returns>
		static Vector<AttributeSignature> Signatures();

	private:
		std::string Class;
		std::string CreateName;
		std::string CreateContext;
	};

	DefineFactory(Action_CreateAction, Action);
}
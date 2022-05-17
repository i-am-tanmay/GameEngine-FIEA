#pragma once
#include "GameTime.h"
#include "Action.h"
#include "GameObject.h"

namespace FIEAGameEngine
{
	class Action_DestroyAction final : public Action
	{
		RTTI_DECLARATIONS(Action_DestroyAction);

	public:
		/// <summary>
		/// Constructor: for Action - Destroy Action
		/// </summary>
		Action_DestroyAction();

		/// <summary>
		/// Destroys Action from given Name, Adds Destroy Task to ActionManager Queue
		/// </summary>
		/// <param name="gametime">Reference to current gametime</param>
		void Update(const GameTime& gametime) override;

		/// <summary>
		/// Clone this Destroy action
		/// </summary>
		/// <returns>Raw Pointer to cloned action</returns>
		virtual gsl::owner<Action_DestroyAction*> Clone() const override;

		/// <summary>
		/// Equality for DestroyAction action
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
		std::string DestroyName;
	};

	DefineFactory(Action_DestroyAction, Action);
}
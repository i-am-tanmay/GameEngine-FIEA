#pragma once
#include "GameTime.h"
#include "ActionList.h"

namespace FIEAGameEngine
{
	class ActionList_IF final : public ActionList
	{
		RTTI_DECLARATIONS(ActionList_IF);

	public:
		/// <summary>
		/// Constructor: for IF Action
		/// </summary>
		ActionList_IF();

		/// <summary>
		/// Calls Appropriate Action based on Condition
		/// </summary>
		/// <param name="gametime">Reference to current gametime</param>
		void Update(const GameTime& gametime) override;

		/// <summary>
		/// Clone this IF action
		/// </summary>
		/// <returns>Raw Pointer to cloned action</returns>
		virtual gsl::owner<ActionList_IF*> Clone() const override;

		/// <summary>
		/// Equality for IF action
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
		/// Get Prescribed Attributes for Action IF Class
		/// </summary>
		/// <returns>Vector of Prescribed Attribute Signatures</returns>
		static Vector<AttributeSignature> Signatures();

	private:
		std::int32_t _condition{ 0 };
	};

	DefineFactory(ActionList_IF, Action);
}
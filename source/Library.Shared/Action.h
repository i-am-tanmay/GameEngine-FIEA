#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "GameTime.h"

namespace FIEAGameEngine
{
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action);

	public:

		/// <summary>
		/// Name for this Action
		/// </summary>
		std::string Name{ "Action" };

		/// <summary>
		/// Update all Actions in this action
		/// </summary>
		/// <param name="gametime">Reference to current gametime</param>
		virtual void Update(const GameTime& gametime) = 0;

		/// <summary>
		/// Get Prescribed Attributes for Action Class
		/// </summary>
		/// <returns>Vector of Prescribed Attribute Signatures</returns>
		static Vector<AttributeSignature> Signatures();

	protected:
		Action(size_type _typeid);
	};
}
#pragma once
#include "GameObject.h"
#include "TypeManager.h"
#include "Vector.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <gsl/gsl>

using namespace FIEAGameEngine;
namespace LibraryDesktopTests
{
	class GOFoo final : public FIEAGameEngine::GameObject
	{
		RTTI_DECLARATIONS(GOFoo);

	public:
		using size_type = std::size_t;

	public:
		GOFoo();

		float testfloat{ 0 };

		/// <summary>
		/// Update GOFoo every frame
		/// </summary>
		/// <param name="gametime">Reference to GameTime for current instance of game</param>
		virtual void Update(const GameTime& gametime);

		/// <summary>
		/// Create a new copy of this GOFoo instance
		/// </summary>
		/// <returns>Pointer to new copied instance</returns>
		gsl::owner<GOFoo*> Clone() const override;

		/// <summary>
		/// Equality check
		/// </summary>
		/// <param name="rhs">Pointer to data to check equality with</param>
		/// <returns>TRUE if they are equal | FALSE if not</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Convert to string
		/// </summary>
		/// <returns>Name of GOFoo</returns>
		std::string ToString() const override;

		/// <summary>
		/// Get Signatures list for GOFoo class
		/// </summary>
		/// <returns>Vector of Signatures of GOFoo</returns>
		static FIEAGameEngine::Vector<FIEAGameEngine::AttributeSignature> Signatures();
	};

	DefineFactory(GOFoo, FIEAGameEngine::Scope);
}

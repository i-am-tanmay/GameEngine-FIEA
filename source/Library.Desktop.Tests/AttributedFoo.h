#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "Vector.h"
#include "Factory.h"
#include "GameObject.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <gsl/gsl>

namespace LibraryDesktopTests
{
	class AttributedFoo : public FIEAGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo)

	public:
		using size_type = std::size_t;

	public:
		static const size_type ArraySize = 5;

		/// <summary>
		/// Constructor: populates list of attributes of attributedfoo
		/// </summary>
		AttributedFoo();

		/// <summary>
		/// Copy construct: using another attributedfoo
		/// </summary>
		/// <param name="">Reference to other attributedfoo to copy from</param>
		AttributedFoo(const AttributedFoo&) = default;

		/// <summary>
		/// Copy assignment: using another attributedfoo
		/// </summary>
		/// <returns>Reference to new this attributedfoo</returns>
		AttributedFoo& operator=(const AttributedFoo&) = default;

		/// <summary>
		/// Move construct: using other attributedfoo
		/// </summary>
		AttributedFoo(AttributedFoo&&) noexcept = default;

		/// <summary>
		/// Move assignment: using other attributedfoo
		/// </summary>
		/// <returns>other attributedfoo to move</returns>
		AttributedFoo& operator=(AttributedFoo&&) noexcept = default;

		virtual ~AttributedFoo() = default;

		int ExternalInteger{ 0 };
		float ExternalFloat{ 0 };
		std::string ExternalString;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArray[ArraySize];
		float ExternalFloatArray[ArraySize];
		std::string ExternalStringArray[ArraySize];
		glm::vec4 ExternalVectorArray[ArraySize];
		glm::mat4 ExternalMatrixArray[ArraySize];

		/// <summary>
		/// Create a new copy of this attributedfoo instance
		/// </summary>
		/// <returns>Pointer to new copied instance</returns>
		virtual gsl::owner<AttributedFoo*> Clone() const override;

		/// <summary>
		/// Equality check
		/// </summary>
		/// <returns>TRUE if they are equal | FALSE if not</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Convert to string
		/// </summary>
		/// <returns>AttributedFoo</returns>
		std::string ToString() const override;

		/// <summary>
		/// Get Signatures list for AttributedFoo class
		/// </summary>
		/// <returns>Vector of Signatures of AttributedFoo</returns>
		static FIEAGameEngine::Vector<FIEAGameEngine::AttributeSignature> Signatures();

	protected:
		/// <summary>
		/// Constructor for Derived classes of AttributedFoo
		/// </summary>
		AttributedFoo(size_type _typeid);
	};

	DefineFactory(AttributedFoo, FIEAGameEngine::Scope);
}
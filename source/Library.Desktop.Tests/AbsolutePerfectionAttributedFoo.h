#pragma once
#include "AttributedFoo.h"
#include "TypeManager.h"
#include "Vector.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <gsl/gsl>

namespace LibraryDesktopTests
{
	class AbsolutePerfectionAttributedFoo : public AttributedFoo
	{
		RTTI_DECLARATIONS(AbsolutePerfectionAttributedFoo)

	public:
		using size_type = std::size_t;

	public:
		static const size_type ArraySize = 5;

		/// <summary>
		/// Constructor: populates list of attributes of AbsolutePerfectionAttributedFoo
		/// </summary>
		AbsolutePerfectionAttributedFoo();

		/// <summary>
		/// Copy construct: using another AbsolutePerfectionAttributedFoo
		/// </summary>
		/// <param name="">Reference to other AbsolutePerfectionAttributedFoo to copy from</param>
		AbsolutePerfectionAttributedFoo(const AbsolutePerfectionAttributedFoo&) = default;

		/// <summary>
		/// Copy assignment: using another AbsolutePerfectionAttributedFoo
		/// </summary>
		/// <returns>Reference to new this AbsolutePerfectionAttributedFoo</returns>
		AbsolutePerfectionAttributedFoo& operator=(const AbsolutePerfectionAttributedFoo&) = default;

		/// <summary>
		/// Move construct: using other AbsolutePerfectionAttributedFoo
		/// </summary>
		AbsolutePerfectionAttributedFoo(AbsolutePerfectionAttributedFoo&&) noexcept = default;

		/// <summary>
		/// Move assignment: using other AbsolutePerfectionAttributedFoo
		/// </summary>
		/// <returns>other AbsolutePerfectionAttributedFoo to move</returns>
		AbsolutePerfectionAttributedFoo& operator=(AbsolutePerfectionAttributedFoo&&) noexcept = default;

		virtual ~AbsolutePerfectionAttributedFoo() = default;

		int ExternalInteger{ 0 };
		float ExternalFloat{ 0 };
		std::string ExternalStringPerfection;
		glm::vec4 ExternalVectorPerfection;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArrayPerfection[ArraySize];
		float ExternalFloatArrayPerfection[ArraySize];
		std::string ExternalStringArray[ArraySize];
		glm::vec4 ExternalVectorArray[ArraySize];
		glm::mat4 ExternalMatrixArray[ArraySize];

		/// <summary>
		/// Create a new copy of this AbsolutePerfectionAttributedFoo instance
		/// </summary>
		/// <returns>Pointer to new copied instance</returns>
		gsl::owner<AbsolutePerfectionAttributedFoo*> Clone() const override;

		/// <summary>
		/// Equality check
		/// </summary>
		/// <returns>TRUE if they are equal | FALSE if not</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Convert to string
		/// </summary>
		/// <returns>AbsolutePerfectionAttributedFoo</returns>
		std::string ToString() const override;

		/// <summary>
		/// Get Signatures list for AbsolutePerfectionAttributedFoo class
		/// </summary>
		/// <returns>Vector of Signatures of AbsolutePerfectionAttributedFoo</returns>
		static FIEAGameEngine::Vector<FIEAGameEngine::AttributeSignature> Signatures();
	};

	class AbsoluteGarbageAttributedFoo : public AttributedFoo
	{
		RTTI_DECLARATIONS(AbsoluteGarbageAttributedFoo)

	public:
		using size_type = std::size_t;

	public:
		static const size_type ArraySize = 5;

		/// <summary>
		/// Constructor: populates list of attributes of AbsoluteGarbageAttributedFoo
		/// </summary>
		AbsoluteGarbageAttributedFoo();
		AbsoluteGarbageAttributedFoo(const AbsoluteGarbageAttributedFoo&) = delete;
		AbsoluteGarbageAttributedFoo& operator=(const AbsoluteGarbageAttributedFoo&) = delete;
		AbsoluteGarbageAttributedFoo(AbsoluteGarbageAttributedFoo&&) noexcept = delete;
		AbsoluteGarbageAttributedFoo& operator=(AbsoluteGarbageAttributedFoo&&) noexcept = delete;

		virtual ~AbsoluteGarbageAttributedFoo() = default;

		float ExternalInteger{ 0 };

		static FIEAGameEngine::Vector<FIEAGameEngine::AttributeSignature> Signatures();
	};
}
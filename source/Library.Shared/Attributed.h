#pragma once
#include <cstdint>
#include <stdexcept>
#include "Vector.h"
#include "Datum.h"
#include "Scope.h"
#include "TypeManager.h"
#include "AttributeSignature.h"

using namespace std::string_literals;
namespace FIEAGameEngine
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed);

	public:
		using size_type = std::size_t;

	public:
		virtual ~Attributed() = default;

		/// <summary>
		/// Add a new Auxiliary Attribute 
		/// </summary>
		/// <param name="">Key for new entry</param>
		/// <returns>Reference to added datum</returns>
		Datum& AppendAuxiliaryAttribute(const std::string&);

		/// <summary>
		/// Is this key an attribute
		/// </summary>
		/// <param name="">Key to check entry of</param>
		/// <returns>TRUE if it is an attribute | FALSE if not</returns>
		bool IsAttribute(const std::string&) const;

		/// <summary>
		/// Is this key a Prescribed Attribute
		/// </summary>
		/// <param name="">Key to check entry of</param>
		/// <returns>TRUE if it is a prescribed attribute | FALSE if not</returns>
		bool IsPrescribedAttribute(const std::string&) const;

		/// <summary>
		/// Is this key an auxiliary attribute
		/// </summary>
		/// <param name="">Key to check entry of</param>
		/// <returns>TRUE if it is an auxiliary attribute | FALSE if not</returns>
		bool IsAuxiliaryAttribute(const std::string&) const;

		/// <summary>
		/// Get Signatures list for Attributed class
		/// </summary>
		/// <returns>empty vector</returns>
		static FIEAGameEngine::Vector<FIEAGameEngine::AttributeSignature> Signatures();

	protected:
		/// <summary>
		/// Constructor: Populates the list of attributes
		/// </summary>
		/// <param name="">TypeID of attributed type</param>
		Attributed(RTTI::IdType);

		/// <summary>
		/// Copy Construct list of attributes
		/// </summary>
		/// <param name="">Reference to other attributed list to copy from</param>
		Attributed(const Attributed&);

		/// <summary>
		/// Copy assignment: list of attributes
		/// </summary>
		/// <param name="">Reference to other attributed list to copy from</param>
		/// <returns>Reference to new this attributed list</returns>
		Attributed& operator=(const Attributed&);

		/// <summary>
		/// Move Construct list of attributes
		/// </summary>
		/// <param name="">Other attributed list to move from</param>
		Attributed(Attributed&&) noexcept;

		/// <summary>
		/// Move assignment list of attributes
		/// </summary>
		/// <param name="">Other attributed list to move from</param>
		/// <returns>Reference to new this attributed list after move</returns>
		Attributed& operator=(Attributed&&) noexcept;

	private:
		void UpdateExternalStorage(const uint8_t* const rhsptr);

	private:
		size_type _prescribedCount{ 1 };
	};
}
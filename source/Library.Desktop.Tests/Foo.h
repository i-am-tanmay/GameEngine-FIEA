#pragma once
#include <RTTI.h>
#include "Factory.h"
#include "GameObject.h"

namespace LibraryDesktopTests
{
	class Foo : public FIEAGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo)

	public:

		/// <summary>
		/// Constructor: Initializes data
		/// </summary>
		/// <param name="data">Initialize using passed value</param>
		Foo(std::int32_t data = 0);

		/// <summary>
		/// Constructor: Deep-Copy data from another Foo
		/// </summary>
		/// <param name="other">Initialize by copying passed Foo</param>
		Foo(const Foo& other);

		/// <summary>
		/// Assignment: Deep-Copy data from another Foo
		/// </summary>
		/// <param name="other">Assign data using passed Foo</param>
		/// <returns>Reference to this Foo</returns>
		Foo& operator=(const Foo& other);

		virtual ~Foo();

		/// <summary>
		/// Get a reference to data
		/// </summary>
		/// <returns>Mutable reference to data of this Foo</returns>
		std::int32_t& Data();

		/// <summary>
		/// Get a constant reference to data
		/// </summary>
		/// <returns>Un-Mutable reference to data of this Foo</returns>
		const std::int32_t& Data() const;

		/// <summary>
		/// AreEqual: Checks if both Foo have same Data
		/// </summary>
		/// <param name="rhs">Other Foo to compare with</param>
		/// <returns>TRUE if both have same data | FALSE if data is different</returns>
		bool operator==(const Foo& rhs) const;

		/// <summary>
		/// AreNotEqual: Checks if both Foo have different Data
		/// </summary>
		/// <param name="rhs">Other Foo to compare with</param>
		/// <returns>TRUE if both have different data | FALSE if data is same</returns>
		bool operator!=(const Foo& rhs) const;

		/// <summary>
		/// ToString overload for RTTI
		/// </summary>
		/// <returns>String data</returns>
		std::string ToString() const override;

	private:
		std::int32_t* mData;
	};

	DefineFactory(Foo, FIEAGameEngine::RTTI);
}
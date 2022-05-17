#include "pch.h"
#include "Foo.h"
#include "SList.h"

namespace LibraryDesktopTests
{
	RTTI_DEFINITIONS(Foo, FIEAGameEngine::RTTI)

	Foo::Foo(std::int32_t data) :
		mData(new std::int32_t(data))
	{
	}

	Foo::Foo(const Foo& other) :
		mData(new std::int32_t(*other.mData))
	{
	}

	Foo& Foo::operator=(const Foo& other)
	{
		if (this != &other)
		{
			*mData = *other.mData;
		}

		return *this;
	}

	Foo::~Foo()
	{
		delete mData;
	}

	std::int32_t& Foo::Data()
	{
		return *mData;
	}

	const std::int32_t& Foo::Data() const
	{
		return *mData;
	}

	bool Foo::operator==(const Foo& rhs) const
	{
		if (mData == rhs.mData)
		{
			return true;
		}

		return (*mData == *rhs.mData);
	}

	bool Foo::operator!=(const Foo& rhs) const
	{
		return !operator==(rhs);
	}

	std::string Foo::ToString() const
	{
		return std::to_string(*mData);
	}
}
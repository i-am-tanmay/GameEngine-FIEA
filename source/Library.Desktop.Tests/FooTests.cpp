#include "pch.h"
#include "Foo.h"
#include "SList.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(FooTests)
	{
	public:

		TEST_METHOD(Constructor)
		{
			Foo f(20);
			Assert::AreEqual(f.Data(), 20);
		}

		TEST_METHOD(CopyConstructor)
		{
			Foo f(20);
			Foo fagain = f;
			Assert::AreEqual(fagain.Data(), 20);
			Assert::AreNotSame(f, fagain);
			Assert::AreNotSame(f.Data(), fagain.Data());
		}

		TEST_METHOD(Data)
		{
			Foo f(20);
			Assert::AreEqual(f.Data(), 20);
			std::int32_t& data = f.Data();
			data = 30;
			Assert::AreEqual(f.Data(), 30);

			const Foo& fconst = f;
			Assert::AreEqual(fconst.Data(), 30);
		}

		TEST_METHOD(EqualOperator)
		{
			Foo f(20);
			Foo fagain(20);
			Assert::AreNotSame(f, fagain);

			Assert::IsTrue(f == f);
			Assert::IsTrue(f == fagain);

			fagain.Data() = 30;
			Assert::IsTrue(f != fagain);
		}
	};
}
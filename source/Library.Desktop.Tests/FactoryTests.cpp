#include "pch.h"
#include "Factory.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(FactoryTests)
	{
	public:

#pragma region Memory Leak Test
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}
#pragma endregion

		TEST_METHOD(footest)
		{
			RTTI* rtti = Factory<RTTI>::Create("Foo");
			Assert::IsNull(rtti);

			{
				FooFactory foofact;

				rtti = Factory<RTTI>::Create("Foo");
				Assert::IsNotNull(rtti);

				Foo* foo = rtti->As<Foo>();
				Assert::IsNotNull(foo);

				delete foo;
			}

			rtti = Factory<RTTI>::Create("Foo");
			Assert::IsNull(rtti);

			const Factory<RTTI>* foundFactory = Factory<RTTI>::Find("Foo"s);
			Assert::IsNull(foundFactory);

			{
				const FooFactory fooFactory;
				foundFactory = Factory<RTTI>::Find("Foo"s);
				Assert::AreEqual(&fooFactory, static_cast<const FooFactory*>(foundFactory));
			}

			foundFactory = Factory<RTTI>::Find("Foo"s);
			Assert::IsNull(foundFactory);

			{
				FooFactory foofact;
				Assert::ExpectException<std::runtime_error>([] {FooFactory foofactory_exception; });
			}
		}

	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState FactoryTests::_startMemState;
}
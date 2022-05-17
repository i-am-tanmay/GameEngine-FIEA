#include "pch.h"
#include "Foo.h"
#include "Datum.h"
#include "RTTI.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(DatumTests)
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

		TEST_METHOD(Constructor)
		{
			{
				Datum dt;
				Assert::AreEqual(dt.Size(), std::size_t(0));
				Assert::AreEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Unknown);
			}

			{
				Datum dt{ DatumTypes::Vector };
				Assert::AreEqual(dt.Size(), std::size_t(0));
				Assert::AreEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Vector);
			}

			{
				Datum dt{ DatumTypes::String, 7u };
				Assert::AreEqual(dt.Size(), std::size_t(0));
				Assert::AreEqual(dt.Capacity(), std::size_t(7));
				Assert::AreEqual(dt.Type(), DatumTypes::String);
			}

			Datum dt_i = 3;
			Assert::AreEqual(dt_i.Size(), std::size_t(1));
			Assert::AreEqual(dt_i.Type(), DatumTypes::Integer);

			Datum dt_f = 4.f;
			Assert::AreEqual(dt_f.Size(), std::size_t(1));
			Assert::AreEqual(dt_f.Type(), DatumTypes::Float);

			glm::vec4 v(1.f, 2.f, 3.f, 4.f);
			Datum dt_v = v;
			Assert::AreEqual(dt_v.Size(), std::size_t(1));
			Assert::AreEqual(dt_v.Type(), DatumTypes::Vector);

			glm::mat4 m(1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f);
			Datum dt_m = m;
			Assert::AreEqual(dt_m.Size(), std::size_t(1));
			Assert::AreEqual(dt_m.Type(), DatumTypes::Matrix);

			Datum dt_s = "Whyyy";
			Assert::AreEqual(dt_s.Size(), std::size_t(1));
			Assert::AreEqual(dt_s.Type(), DatumTypes::String);

			Foo foo{ 10 };
			Datum dt_r = &foo;
			Assert::AreEqual(dt_r.Size(), std::size_t(1));
			Assert::AreEqual(dt_r.Type(), DatumTypes::Pointer);
		}

		TEST_METHOD(AssignmentOp)
		{
			{
				Datum dt;
				dt = 3;
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Integer);
				Assert::AreEqual(std::int32_t(3), dt.Get<std::int32_t>());
			}

			{
				Datum dt;
				dt = 9.f;
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Float);
				Assert::AreEqual(9.f, dt.Get<float>());
			}

			{
				Datum dt;
				glm::vec4 v(1.f, 2.f, 3.f, 4.f);
				dt = v;
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Vector);
				Assert::AreEqual(v, dt.Get<glm::vec4>());
			}

			{
				Datum dt;
				glm::mat4 m(1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f);
				dt = m;
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Matrix);
				Assert::AreEqual(m, dt.Get<glm::mat4>());
			}

			{
				Datum dt;
				dt = "Whyyyy";
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::String);
				Assert::AreEqual(std::string("Whyyyy"), dt.Get<std::string>());
			}

			{
				Foo foo{ 10 };
				RTTI* r = &foo;
				Datum dt;
				dt = r;
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Pointer);
				Assert::AreEqual(r, dt.Get<RTTI*>());
			}
		}

		TEST_METHOD(CopySymantics)
		{
			{
				Datum dt = 4;
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Integer);
				Assert::AreEqual(std::int32_t(4), dt.Get<std::int32_t>());

				{
					Datum dtcopy = dt;
					Assert::AreEqual(dt.Size(), std::size_t(1));
					Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::Integer);
					Assert::AreEqual(std::int32_t(4), dt.Get<std::int32_t>());
					Assert::AreEqual(dtcopy.Size(), std::size_t(1));
					Assert::AreNotEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::Integer);
					Assert::AreEqual(std::int32_t(4), dtcopy.Get<std::int32_t>());
				}

				{
					Datum dtcopy;
					Assert::AreEqual(dtcopy.Size(), std::size_t(0));
					Assert::AreEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::Unknown);
					dtcopy = dt;
					Assert::AreEqual(dt.Size(), std::size_t(1));
					Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::Integer);
					Assert::AreEqual(std::int32_t(4), dt.Get<std::int32_t>());
					Assert::AreEqual(dtcopy.Size(), std::size_t(1));
					Assert::AreNotEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::Integer);
					Assert::AreEqual(std::int32_t(4), dtcopy.Get<std::int32_t>());
				}
			}

			{
				Datum dt = "Whyyyyyyyyyy";
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::String);
				Assert::AreEqual(std::string("Whyyyyyyyyyy"), dt.Get<std::string>());

				{
					Datum dtcopy = dt;
					Assert::AreEqual(dt.Size(), std::size_t(1));
					Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::String);
					Assert::AreEqual(std::string("Whyyyyyyyyyy"), dt.Get<std::string>());
					Assert::AreEqual(dtcopy.Size(), std::size_t(1));
					Assert::AreNotEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::String);
					Assert::AreEqual(std::string("Whyyyyyyyyyy"), dtcopy.Get<std::string>());
				}

				{
					Datum dtcopy;
					Assert::AreEqual(dtcopy.Size(), std::size_t(0));
					Assert::AreEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::Unknown);
					dtcopy = dt;
					Assert::AreEqual(dt.Size(), std::size_t(1));
					Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::String);
					Assert::AreEqual(std::string("Whyyyyyyyyyy"), dt.Get<std::string>());
					Assert::AreEqual(dtcopy.Size(), std::size_t(1));
					Assert::AreNotEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::String);
					Assert::AreEqual(std::string("Whyyyyyyyyyy"), dtcopy.Get<std::string>());
				}
			}

			{
				int x = 10;

				Datum dt;
				dt.SetStorageExternal(&x, 1);

				Datum dtcopy = dt;
				Assert::IsTrue(dtcopy.IsExternalStorage());

				Datum dtext;
				dtext = dt;
				Assert::IsTrue(dtext.IsExternalStorage());

				Datum dtint = 10;
				dtext = dtint;
				Assert::IsFalse(dtext.IsExternalStorage());

				Datum dtints = "whyyyyyyyyyyyyyy";
				dtcopy = dtints;
				Assert::IsFalse(dtcopy.IsExternalStorage());
			}
			
		}

		TEST_METHOD(MoveSymantics)
		{
			{
				{
					Datum dt = 4;
					Assert::AreEqual(dt.Size(), std::size_t(1));
					Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::Integer);
					Assert::AreEqual(std::int32_t(4), dt.Get<std::int32_t>());

					Datum dtcopy = std::move(dt);
					Assert::AreEqual(dt.Size(), std::size_t(0));
					Assert::AreEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::Unknown);
					Assert::AreEqual(dtcopy.Size(), std::size_t(1));
					Assert::AreNotEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::Integer);
					Assert::AreEqual(std::int32_t(4), dtcopy.Get<std::int32_t>());
				}

				{
					Datum dt = 4;
					Assert::AreEqual(dt.Size(), std::size_t(1));
					Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::Integer);
					Assert::AreEqual(std::int32_t(4), dt.Get<std::int32_t>());

					Datum dtcopy;
					Assert::AreEqual(dtcopy.Size(), std::size_t(0));
					Assert::AreEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::Unknown);
					dtcopy = std::move(dt);
					Assert::AreEqual(dt.Size(), std::size_t(0));
					Assert::AreEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::Unknown);
					Assert::AreEqual(dtcopy.Size(), std::size_t(1));
					Assert::AreNotEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::Integer);
					Assert::AreEqual(std::int32_t(4), dtcopy.Get<std::int32_t>());
				}
			}

			{
				{
					Datum dt = "Whyyyyyyyyyy";
					Assert::AreEqual(dt.Size(), std::size_t(1));
					Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::String);
					Assert::AreEqual(std::string("Whyyyyyyyyyy"), dt.Get<std::string>());

					Datum dtcopy = std::move(dt);
					Assert::AreEqual(dt.Size(), std::size_t(0));
					Assert::AreEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::Unknown);
					Assert::AreEqual(dtcopy.Size(), std::size_t(1));
					Assert::AreNotEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::String);
					Assert::AreEqual(std::string("Whyyyyyyyyyy"), dtcopy.Get<std::string>());
				}

				{
					Datum dt = "Whyyyyyyyyyy";
					Assert::AreEqual(dt.Size(), std::size_t(1));
					Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::String);
					Assert::AreEqual(std::string("Whyyyyyyyyyy"), dt.Get<std::string>());

					Datum dtcopy;
					Assert::AreEqual(dtcopy.Size(), std::size_t(0));
					Assert::AreEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::Unknown);
					dtcopy = std::move(dt);
					Assert::AreEqual(dt.Size(), std::size_t(0));
					Assert::AreEqual(dt.Capacity(), std::size_t(0));
					Assert::AreEqual(dt.Type(), DatumTypes::Unknown);
					Assert::AreEqual(dtcopy.Size(), std::size_t(1));
					Assert::AreNotEqual(dtcopy.Capacity(), std::size_t(0));
					Assert::AreEqual(dtcopy.Type(), DatumTypes::String);
					Assert::AreEqual(std::string("Whyyyyyyyyyy"), dtcopy.Get<std::string>());
				}
			}
		}

		TEST_METHOD(Assignment)
		{
			{
				std::int32_t a = 1;
				std::int32_t b = 2;

				Datum dta = a;
				Datum dtb = b;
				Assert::IsTrue(dta != dtb);
				Assert::IsTrue(dtb != a);

				dtb = a;
				Assert::IsTrue(dta == dtb);
				Assert::IsTrue(dtb == a);
			}

			{
				float a = 1.f;
				float b = 2.f;

				Datum dta = a;
				Datum dtb = b;
				Assert::IsTrue(dta != dtb);
				Assert::IsTrue(dtb != a);

				dtb = a;
				Assert::IsTrue(dta == dtb);
				Assert::IsTrue(dtb == a);
			}

			{
				glm::vec4 a{ 1.0f };
				glm::vec4 b{ 2.0f };

				Datum dta = a;
				Datum dtb = b;
				Assert::IsTrue(dta != dtb);
				Assert::IsTrue(dtb != a);

				dtb = a;
				Assert::IsTrue(dta == dtb);
				Assert::IsTrue(dtb == a);
			}

			{
				glm::mat4 a{ 1.0f };
				glm::mat4 b{ 2.0f };

				Datum dta = a;
				Datum dtb = b;
				Assert::IsTrue(dta != dtb);
				Assert::IsTrue(dtb != a);

				dtb = a;
				Assert::IsTrue(dta == dtb);
				Assert::IsTrue(dtb == a);
			}

			{
				std::string a = "whyyyy";
				std::string b = "why?";

				Datum dta = a;
				Datum dtb = b;
				Assert::IsTrue(dta != dtb);
				Assert::IsTrue(dtb != a);

				dtb = a;
				Assert::IsTrue(dta == dtb);
				Assert::IsTrue(dtb == a);
			}

			{
				Foo aFoo{ 1 };
				Foo bFoo{ 2 };

				RTTI* a = &aFoo;
				RTTI* b = &bFoo;

				Datum dta = a;
				Datum dtb = b;
				Assert::IsTrue(dta != dtb);
				Assert::IsTrue(dtb != a);

				dtb = a;
				Assert::IsTrue(dta == dtb);
				Assert::IsTrue(dtb == a);
			}
		}

		TEST_METHOD(SetType)
		{
			Datum dt;
			Assert::AreEqual(DatumTypes::Unknown, dt.Type());

			dt.SetType(DatumTypes::Integer);
			Assert::AreEqual(DatumTypes::Integer, dt.Type());
			Assert::IsFalse(dt.SetType(DatumTypes::Float));
			Assert::IsFalse(dt.SetType(DatumTypes::Unknown));
		}

		TEST_METHOD(Reserve)
		{
			{
				Datum dt;
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(0), dt.Capacity());

				Assert::ExpectException<std::runtime_error>([&dt] {dt.Reserve(5); });

				dt.SetType(DatumTypes::Integer);
				dt.Reserve(5);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());

				dt.Reserve(3);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());
			}

			{
				Datum dt;
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(0), dt.Capacity());

				dt.SetType(DatumTypes::Float);
				dt.Reserve(5);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());

				dt.Reserve(3);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());
			}

			{
				Datum dt;
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(0), dt.Capacity());

				dt.SetType(DatumTypes::Vector);
				dt.Reserve(5);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());

				dt.Reserve(3);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());
			}

			{
				Datum dt;
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(0), dt.Capacity());

				dt.SetType(DatumTypes::Matrix);
				dt.Reserve(5);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());

				dt.Reserve(3);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());
			}

			{
				Datum dt;
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(0), dt.Capacity());

				dt.SetType(DatumTypes::String);
				dt.Reserve(5);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());

				dt.Reserve(3);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());
			}

			{
				Datum dt;
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(0), dt.Capacity());

				dt.SetType(DatumTypes::Pointer);
				dt.Reserve(5);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());

				dt.Reserve(3);
				Assert::AreEqual(std::size_t(0), dt.Size());
				Assert::AreEqual(std::size_t(5), dt.Capacity());
			}
		}

		TEST_METHOD(Resize)
		{
			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.ReSize(5); });

				dtm.SetType(DatumTypes::Integer);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.ReSize(3);
				Assert::AreEqual(std::size_t(3), dtm.Size());
				Assert::AreEqual(std::size_t(3), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Float);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.ReSize(3);
				Assert::AreEqual(std::size_t(3), dtm.Size());
				Assert::AreEqual(std::size_t(3), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Vector);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.ReSize(3);
				Assert::AreEqual(std::size_t(3), dtm.Size());
				Assert::AreEqual(std::size_t(3), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Matrix);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.ReSize(3);
				Assert::AreEqual(std::size_t(3), dtm.Size());
				Assert::AreEqual(std::size_t(3), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::String);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.ReSize(3);
				Assert::AreEqual(std::size_t(3), dtm.Size());
				Assert::AreEqual(std::size_t(3), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Pointer);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.ReSize(3);
				Assert::AreEqual(std::size_t(3), dtm.Size());
				Assert::AreEqual(std::size_t(3), dtm.Capacity());
			}
		}

		TEST_METHOD(Clear)
		{
			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Integer);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Float);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Vector);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Matrix);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::String);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Pointer);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());
			}
		}

		TEST_METHOD(ShrinkToFit)
		{
			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Integer);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				dtm.PushBack(2);
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(std::size_t(1), dtm.Capacity());

				dtm.Clear();
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Float);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				dtm.PushBack(2.f);
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(std::size_t(1), dtm.Capacity());

				dtm.Clear();
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Vector);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				dtm.PushBack(glm::vec4(2.f));
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(std::size_t(1), dtm.Capacity());

				dtm.Clear();
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Matrix);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				dtm.PushBack(glm::mat4(2.f));
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(std::size_t(1), dtm.Capacity());

				dtm.Clear();
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::String);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				dtm.Clear();
				dtm.PushBack("Whyyyyyy");
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(std::size_t(1), dtm.Capacity());

				dtm.Clear();
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Pointer);
				dtm.ReSize(5);
				Assert::AreEqual(std::size_t(5), dtm.Size());
				Assert::AreEqual(std::size_t(5), dtm.Capacity());

				Foo foo{ 10 };
				RTTI* r = &foo;
				dtm.Clear();
				dtm.PushBack(r);
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(std::size_t(1), dtm.Capacity());

				dtm.Clear();
				dtm.ShrinkToFit();
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());
			}
		}

		TEST_METHOD(FromString)
		{
			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Integer);
				dtm.Reserve(1);
				Assert::IsTrue(dtm.PushBackFromString("2"));
				Assert::IsFalse(dtm.PushBackFromString("sdasfav"));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(2, dtm.Get<std::int32_t>());
				Assert::IsTrue(dtm.SetFromString("4"));
				Assert::IsFalse(dtm.SetFromString("sadnas"));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(4, dtm.Get<std::int32_t>());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Float);
				dtm.Reserve(1);
				Assert::IsTrue(dtm.PushBackFromString("3.f"));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(3.f, dtm.Get<float>());
				Assert::IsTrue(dtm.SetFromString("9.f"));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(9.f, dtm.Get<float>());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Vector);
				dtm.Reserve(1);
				Assert::IsTrue(dtm.PushBackFromString(glm::to_string(glm::vec4(4.f))));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(glm::vec4(4.f), dtm.Get<glm::vec4>());
				Assert::IsTrue(dtm.SetFromString(glm::to_string(glm::vec4(2.f))));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(glm::vec4(2.f), dtm.Get<glm::vec4>());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::Matrix);
				dtm.Reserve(1);
				Assert::IsTrue(dtm.PushBackFromString(glm::to_string(glm::mat4(4.f))));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(glm::mat4(4.f), dtm.Get<glm::mat4>());
				Assert::IsTrue(dtm.SetFromString(glm::to_string(glm::mat4(2.f))));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(glm::mat4(2.f), dtm.Get<glm::mat4>());
			}

			{
				Datum dtm;
				Assert::AreEqual(std::size_t(0), dtm.Size());
				Assert::AreEqual(std::size_t(0), dtm.Capacity());

				dtm.SetType(DatumTypes::String);
				dtm.Reserve(1);
				Assert::IsTrue(dtm.PushBackFromString("why?"));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(std::string("why?"), dtm.Get<std::string>());
				Assert::IsTrue(dtm.SetFromString("whyyyy"));
				Assert::AreEqual(std::size_t(1), dtm.Size());
				Assert::AreEqual(std::string("whyyyy"), dtm.Get<std::string>());
			}
		}

		TEST_METHOD(ToString)
		{
			{
				Datum dt;
				dt = 3;
				Assert::AreEqual(dt.ToString(), std::string("3"));
			}

			{
				Datum dt;
				dt = 9.f;
				Assert::AreEqual(dt.ToString(), std::to_string(9.f));
			}

			{
				Datum dt;
				glm::vec4 v(1.f, 2.f, 3.f, 4.f);
				dt = v;
				Assert::AreEqual(dt.ToString(), glm::to_string(v));
			}

			{
				Datum dt;
				glm::mat4 m(1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f);
				dt = m;
				Assert::AreEqual(dt.ToString(), glm::to_string(m));
			}

			{
				Datum dt = "Whyyyy";
				Assert::AreEqual(dt.ToString(), std::string("Whyyyy"));
			}

			{
				Foo foo{ 30 };
				RTTI* r = &foo;

				Datum dt = r;
				Assert::AreEqual(std::string("30"), dt.ToString());
			}

			{
				Datum dt;
				Assert::ExpectException<std::out_of_range>([&dt] {dt.ToString(); });
			}
		}

		TEST_METHOD(Get)
		{
			{
				Datum dtm = 2;
				Assert::AreEqual(2, dtm.Get<std::int32_t>());
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Get<std::int32_t>(10); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(2, dtmconst.Get<std::int32_t>());
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Get<std::int32_t>(10); });
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm = 3.f;
				Assert::AreEqual(3.f, dtm.Get<float>());
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Get<float>(10); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<std::int32_t>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(3.f, dtmconst.Get<float>());
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Get<float>(10); });
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<std::int32_t>(); });
			}

			{
				Datum dtm = glm::vec4(4.f);
				Assert::AreEqual(glm::vec4(4.f), dtm.Get<glm::vec4>());
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Get<glm::vec4>(10); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(glm::vec4(4.f), dtmconst.Get<glm::vec4>());
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Get<glm::vec4>(10); });
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm = glm::mat4(4.f);
				Assert::AreEqual(glm::mat4(4.f), dtm.Get<glm::mat4>());
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Get<glm::mat4>(10); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(glm::mat4(4.f), dtmconst.Get<glm::mat4>());
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Get<glm::mat4>(10); });
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm = "why?";
				Assert::AreEqual(std::string("why?"), dtm.Get<std::string>());
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Get<std::string>(10); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(std::string("why?"), dtmconst.Get<std::string>());
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Get<std::string>(10); });
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Foo foo{ 20 };
				RTTI* r = &foo;

				Datum dtm = r;
				Assert::AreEqual(r, dtm.Get<RTTI*>());
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Get<RTTI*>(10); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(r, dtmconst.Get<RTTI*>());
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Get<RTTI*>(10); });
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}
		}

		TEST_METHOD(Front)
		{
			{
				Datum dtm = 2;
				Assert::AreEqual(2, dtm.Front<std::int32_t>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(2, dtmconst.Front<std::int32_t>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm = 3.f;
				Assert::AreEqual(3.f, dtm.Front<float>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<std::int32_t>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(3.f, dtmconst.Front<float>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<std::int32_t>(); });
			}

			{
				Datum dtm = glm::vec4(4.f);
				Assert::AreEqual(glm::vec4(4.f), dtm.Front<glm::vec4>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(glm::vec4(4.f), dtmconst.Front<glm::vec4>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm = glm::mat4(4.f);
				Assert::AreEqual(glm::mat4(4.f), dtm.Front<glm::mat4>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(glm::mat4(4.f), dtmconst.Front<glm::mat4>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm = "why?";
				Assert::AreEqual(std::string("why?"), dtm.Front<std::string>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(std::string("why?"), dtmconst.Front<std::string>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Foo foo{ 20 };
				RTTI* r = &foo;

				Datum dtm = r;
				Assert::AreEqual(r, dtm.Front<RTTI*>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(r, dtmconst.Front<RTTI*>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm;
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Front<std::int32_t>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Front<float>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Front<glm::vec4>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Front<glm::mat4>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Front<std::string>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Front<RTTI*>(); });

				const Datum dtmconst = dtm;
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Front<std::int32_t>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Front<float>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Front<glm::vec4>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Front<glm::mat4>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Front<std::string>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Front<RTTI*>(); });
			}
		}

		TEST_METHOD(Back)
		{
			{
				Datum dtm = 2;
				dtm.PushBack(4);
				Assert::AreEqual(4, dtm.Back<std::int32_t>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(4, dtmconst.Back<std::int32_t>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm = 3.f;
				dtm.PushBack(42.f);
				Assert::AreEqual(42.f, dtm.Back<float>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<std::int32_t>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(42.f, dtmconst.Back<float>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<std::int32_t>(); });
			}

			{
				Datum dtm = glm::vec4(4.f);
				dtm.PushBack(glm::vec4(10.f));
				Assert::AreEqual(glm::vec4(10.f), dtm.Back<glm::vec4>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(glm::vec4(10.f), dtmconst.Back<glm::vec4>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm = glm::mat4(4.f);
				dtm.PushBack(glm::mat4(9.f));
				Assert::AreEqual(glm::mat4(9.f), dtm.Back<glm::mat4>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(glm::mat4(9.f), dtmconst.Back<glm::mat4>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm = "why?";
				dtm.PushBack("whyyyyyyy");
				Assert::AreEqual(std::string("whyyyyyyy"), dtm.Back<std::string>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(std::string("whyyyyyyy"), dtmconst.Back<std::string>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Foo foo{ 20 };
				Foo fooo{ 30 };
				RTTI* r = &foo;
				RTTI* rr = &fooo;

				Datum dtm = r;
				dtm.PushBack(rr);
				Assert::AreEqual(rr, dtm.Back<RTTI*>());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Get<float>(); });

				const Datum dtmconst = dtm;
				Assert::AreEqual(rr, dtmconst.Back<RTTI*>());
				Assert::ExpectException<std::runtime_error>([&dtmconst] {dtmconst.Get<float>(); });
			}

			{
				Datum dtm;
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Back<std::int32_t>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Back<float>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Back<glm::vec4>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Back<glm::mat4>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Back<std::string>(); });
				Assert::ExpectException<std::out_of_range>([&dtm] {dtm.Back<RTTI*>(); });

				const Datum dtmconst = dtm;
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Back<std::int32_t>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Back<float>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Back<glm::vec4>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Back<glm::mat4>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Back<std::string>(); });
				Assert::ExpectException<std::out_of_range>([&dtmconst] {dtmconst.Back<RTTI*>(); });
			}
		}

		TEST_METHOD(PushBack)
		{
			{
				Datum dtm = 2;
				dtm.PushBack(4);
				Assert::AreEqual(4, dtm.Get<std::int32_t>(1));
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack(10.f); });
			}

			{
				Datum dtm = 3.f;
				dtm.PushBack(42.f);
				Assert::AreEqual(42.f, dtm.Get<float>(1));
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack(glm::vec4(4.f)); });
			}

			{
				Datum dtm = glm::vec4(4.f);
				dtm.PushBack(glm::vec4(10.f));
				Assert::AreEqual(glm::vec4(10.f), dtm.Get<glm::vec4>(1));
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack(glm::mat4(4.f)); });
			}

			{
				Datum dtm = glm::mat4(4.f);
				dtm.PushBack(glm::mat4(9.f));
				Assert::AreEqual(glm::mat4(9.f), dtm.Get<glm::mat4>(1));
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack(std::string("asdjbaskd")); });
			}

			{
				Datum dtm = "why?";
				dtm.PushBack("whyyyyyyy");
				Assert::AreEqual(std::string("whyyyyyyy"), dtm.Get<std::string>(1));
				Assert::ExpectException<std::runtime_error>([&dtm] {RTTI* r{ nullptr }; dtm.PushBack(r); });
			}

			{
				Foo foo{ 10 };
				RTTI* r = &foo;

				Datum dtm;
				dtm.SetStorageExternal(&r, 1);

				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack(10); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack(std::int32_t(10)); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack(10.f); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack(glm::vec4(10.f)); });;
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack(glm::mat4(10.f)); });;
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.PushBack("sada"); });

				int x = 20;
				dtm.SetStorageExternal(&x, 1);
				Assert::ExpectException<std::runtime_error>([&dtm, &r] {dtm.PushBack(r); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Clear(); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.ShrinkToFit(); });
			}
		}

		TEST_METHOD(PopBack)
		{
			{
				Datum dtm = 2;
				dtm.PushBack(4);
				Assert::AreEqual(std::size_t(2), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(0), dtm.Size());
			}

			{
				Datum dtm = 3.f;
				dtm.PushBack(42.f);
				Assert::AreEqual(std::size_t(2), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(0), dtm.Size());
			}

			{
				Datum dtm = glm::vec4(4.f);
				dtm.PushBack(glm::vec4(10.f));
				Assert::AreEqual(std::size_t(2), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(0), dtm.Size());
			}

			{
				Datum dtm = glm::mat4(4.f);
				dtm.PushBack(glm::mat4(9.f));
				Assert::AreEqual(std::size_t(2), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(0), dtm.Size());
			}

			{
				Datum dtm = "why?";
				dtm.PushBack("whyyyyyyy");
				Assert::AreEqual(std::size_t(2), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(1), dtm.Size());
				dtm.PopBack();
				Assert::AreEqual(std::size_t(0), dtm.Size());
			}
		}

		TEST_METHOD(Find)
		{
			{
				Datum dtm = 2;
				dtm.PushBack(4);
				Assert::AreEqual(std::size_t(0), dtm.Find(2));
				Assert::AreEqual(std::size_t(1), dtm.Find(4));
				Assert::AreEqual(std::size_t(2), dtm.Find(42));
			}

			{
				Datum dtm = 3.f;
				dtm.PushBack(42.f);
				Assert::AreEqual(std::size_t(0), dtm.Find(3.f));
				Assert::AreEqual(std::size_t(1), dtm.Find(42.f));
				Assert::AreEqual(std::size_t(2), dtm.Find(422.f));
			}

			{
				Datum dtm = glm::vec4(4.f);
				dtm.PushBack(glm::vec4(10.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::vec4(4.f)));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::vec4(10.f)));
				Assert::AreEqual(std::size_t(2), dtm.Find(glm::vec4(422.f)));
			}

			{
				Datum dtm = glm::mat4(4.f);
				dtm.PushBack(glm::mat4(9.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::mat4(4.f)));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::mat4(9.f)));
				Assert::AreEqual(std::size_t(2), dtm.Find(glm::mat4(42.f)));
			}

			{
				Datum dtm = "why?";
				dtm.PushBack("whyyyyyyy");
				Assert::AreEqual(std::size_t(0), dtm.Find("why?"));
				Assert::AreEqual(std::size_t(1), dtm.Find("whyyyyyyy"));
				Assert::AreEqual(std::size_t(2), dtm.Find("whysss?"));
			}

			{
				Foo foo1{ 10 };
				Foo foo2{ 20 };
				Foo foo3{ 30 };

				RTTI* r1 = &foo1;
				RTTI* r2 = &foo2;
				RTTI* r3 = &foo3;

				Datum dtm = r1;
				dtm.PushBack(r2);
				Assert::AreEqual(std::size_t(0), dtm.Find(r1));
				Assert::AreEqual(std::size_t(1), dtm.Find(r2));
				Assert::AreEqual(std::size_t(2), dtm.Find(r3));
			}
		}

		TEST_METHOD(Remove)
		{
			{
				Datum dtm = 2;
				dtm.PushBack(4);
				Assert::AreEqual(std::size_t(0), dtm.Find(2));
				Assert::AreEqual(std::size_t(1), dtm.Find(4));
				dtm.Remove(2);
				Assert::AreEqual(std::size_t(1), dtm.Find(2));
				dtm.Remove(4);
				Assert::AreEqual(std::size_t(0), dtm.Find(4));
				Assert::AreEqual(std::size_t(0), dtm.Find(42));
			}

			{
				Datum dtm = 3.f;
				dtm.PushBack(42.f);
				Assert::AreEqual(std::size_t(0), dtm.Find(3.f));
				Assert::AreEqual(std::size_t(1), dtm.Find(42.f));
				dtm.Remove(3.f);
				Assert::AreEqual(std::size_t(1), dtm.Find(3.f));
				dtm.Remove(42.f);
				Assert::AreEqual(std::size_t(0), dtm.Find(42.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(422.f));
			}

			{
				Datum dtm = glm::vec4(4.f);
				dtm.PushBack(glm::vec4(10.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::vec4(4.f)));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::vec4(10.f)));
				dtm.Remove(glm::vec4(4.f));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::vec4(4.f)));
				dtm.Remove(glm::vec4(10.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::vec4(10.f)));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::vec4(422.f)));
			}

			{
				Datum dtm = glm::mat4(4.f);
				dtm.PushBack(glm::mat4(9.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::mat4(4.f)));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::mat4(9.f)));
				dtm.Remove(glm::mat4(4.f));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::mat4(4.f)));
				dtm.Remove(glm::mat4(9.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::mat4(9.f)));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::mat4(42.f)));
			}

			{
				Datum dtm = "why?";
				dtm.PushBack("whyyyyyyy");
				Assert::AreEqual(std::size_t(0), dtm.Find("why?"));
				Assert::AreEqual(std::size_t(1), dtm.Find("whyyyyyyy"));
				dtm.Remove("why?");
				Assert::AreEqual(std::size_t(1), dtm.Find("why?"));
				dtm.Remove("whyyyyyyy");
				Assert::AreEqual(std::size_t(0), dtm.Find("whyyyyyyy"));
				Assert::AreEqual(std::size_t(0), dtm.Find("whysss?"));
			}

			{
				Foo foo1{ 10 };
				Foo foo2{ 20 };
				Foo foo3{ 30 };

				RTTI* r1 = &foo1;
				RTTI* r2 = &foo2;
				RTTI* r3 = &foo3;

				Datum dtm = r1;
				dtm.PushBack(r2);
				Assert::AreEqual(std::size_t(0), dtm.Find(r1));
				Assert::AreEqual(std::size_t(1), dtm.Find(r2));
				Assert::AreEqual(std::size_t(2), dtm.Find(r3));

				dtm.Remove(r1);
				Assert::AreEqual(std::size_t(1), dtm.Find(r1));
				dtm.Remove(r2);
				Assert::AreEqual(std::size_t(0), dtm.Find(r2));
			}
		}

		TEST_METHOD(RemoveAt)
		{
			{
				Datum dtm = 2;
				dtm.PushBack(4);
				Assert::AreEqual(std::size_t(0), dtm.Find(2));
				Assert::AreEqual(std::size_t(1), dtm.Find(4));
				Assert::IsTrue(dtm.RemoveAt(1));
				Assert::AreEqual(std::size_t(1), dtm.Find(4));
				Assert::IsTrue(dtm.RemoveAt(0));
				Assert::AreEqual(std::size_t(0), dtm.Find(2));
				Assert::AreEqual(std::size_t(0), dtm.Find(42));
			}

			{
				Datum dtm = 3.f;
				dtm.PushBack(42.f);
				Assert::AreEqual(std::size_t(0), dtm.Find(3.f));
				Assert::AreEqual(std::size_t(1), dtm.Find(42.f));
				Assert::IsTrue(dtm.RemoveAt(1));
				Assert::AreEqual(std::size_t(1), dtm.Find(42.f));
				Assert::IsTrue(dtm.RemoveAt(0));
				Assert::AreEqual(std::size_t(0), dtm.Find(3.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(422.f));
			}

			{
				Datum dtm = glm::vec4(4.f);
				dtm.PushBack(glm::vec4(10.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::vec4(4.f)));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::vec4(10.f)));
				Assert::IsTrue(dtm.RemoveAt(1));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::vec4(10.f)));
				Assert::IsTrue(dtm.RemoveAt(0));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::vec4(4.f)));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::vec4(422.f)));
			}

			{
				Datum dtm = glm::mat4(4.f);
				dtm.PushBack(glm::mat4(9.f));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::mat4(4.f)));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::mat4(9.f)));
				Assert::IsTrue(dtm.RemoveAt(1));
				Assert::AreEqual(std::size_t(1), dtm.Find(glm::mat4(9.f)));
				Assert::IsTrue(dtm.RemoveAt(0));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::mat4(4.f)));
				Assert::AreEqual(std::size_t(0), dtm.Find(glm::mat4(42.f)));
			}

			{
				Datum dtm = "why?";
				dtm.PushBack("whyyyyyyy");
				Assert::AreEqual(std::size_t(0), dtm.Find("why?"));
				Assert::AreEqual(std::size_t(1), dtm.Find("whyyyyyyy"));
				Assert::IsTrue(dtm.RemoveAt(1));
				Assert::AreEqual(std::size_t(1), dtm.Find("whyyyyyyy"));
				Assert::IsTrue(dtm.RemoveAt(0));
				Assert::AreEqual(std::size_t(0), dtm.Find("why?"));
				Assert::AreEqual(std::size_t(0), dtm.Find("whysss?"));
				Assert::IsFalse(dtm.RemoveAt(1));
			}
		}

		TEST_METHOD(SetStorage)
		{
			{
				int r = 10;

				Datum dtm;
				dtm.SetStorageExternal(&r, 1);
				Assert::IsTrue(dtm.IsExternalStorage());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Reserve(5); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.ReSize(5); });
			}

			{
				float r = 101.f;

				Datum dtm;
				dtm.SetStorageExternal(&r, 1);
				Assert::IsTrue(dtm.IsExternalStorage());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Reserve(5); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.ReSize(5); });
			}

			{
				glm::vec4 r{ 12.f };

				Datum dtm;
				dtm.SetStorageExternal(&r, 1);
				Assert::IsTrue(dtm.IsExternalStorage());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Reserve(5); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.ReSize(5); });
			}

			{
				glm::mat4 r{ 19.f };

				Datum dtm;
				dtm.SetStorageExternal(&r, 1);
				Assert::IsTrue(dtm.IsExternalStorage());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Reserve(5); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.ReSize(5); });
			}

			{
				std::string r{ "whyyy" };

				Datum dtm;
				dtm.SetStorageExternal(&r, 1);
				Assert::IsTrue(dtm.IsExternalStorage());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Reserve(5); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.ReSize(5); });
			}

			{
				Foo foo{ 10 };
				RTTI* r = &foo;

				Datum dtm;
				dtm.SetStorageExternal(&r, 1);
				Assert::IsTrue(dtm.IsExternalStorage());
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.Reserve(5); });
				Assert::ExpectException<std::runtime_error>([&dtm] {dtm.ReSize(5); });
			}
		}

		TEST_METHOD(Set)
		{
			{
				Datum dt;
				dt.Set(3);
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Integer);
				Assert::AreEqual(std::int32_t(3), dt.Get<std::int32_t>());
				Assert::ExpectException<std::runtime_error>([&dt] {dt.Set(5.f); });
				Assert::ExpectException<std::out_of_range>([&dt] {dt.Set(3, 10); });
			}

			{
				Datum dt;
				dt.Set(9.f);
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Float);
				Assert::AreEqual(9.f, dt.Get<float>());
				Assert::ExpectException<std::runtime_error>([&dt] {dt.Set(glm::vec4(1.f)); });
				Assert::ExpectException<std::out_of_range>([&dt] {dt.Set(3.f, 10); });
			}

			{
				Datum dt;
				glm::vec4 v(1.f, 2.f, 3.f, 4.f);
				dt.Set(v);
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Vector);
				Assert::AreEqual(v, dt.Get<glm::vec4>());
				Assert::ExpectException<std::runtime_error>([&dt] {dt.Set(glm::mat4(1.f)); });
				Assert::ExpectException<std::out_of_range>([&dt, &v] {dt.Set(v, 10); });
			}

			{
				Datum dt;
				glm::mat4 m(1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f);
				dt.Set(m);
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Matrix);
				Assert::AreEqual(m, dt.Get<glm::mat4>());
				Assert::ExpectException<std::runtime_error>([&dt] {dt.Set("asfsdf"); });
				Assert::ExpectException<std::out_of_range>([&dt, &m] {dt.Set(m, 10); });
			}

			{
				Datum dt;
				dt.Set("Whyyyy");
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::String);
				Assert::AreEqual(std::string("Whyyyy"), dt.Get<std::string>());
				Assert::ExpectException<std::runtime_error>([&dt] { RTTI* r{ nullptr };  dt.Set(r); });
				Assert::ExpectException<std::out_of_range>([&dt] {dt.Set("uiasbds", 10); });
			}

			{
				Foo foo{ 10 };
				RTTI* r = &foo;
				Datum dt;
				dt.Set(r);
				Assert::AreEqual(dt.Size(), std::size_t(1));
				Assert::AreNotEqual(dt.Capacity(), std::size_t(0));
				Assert::AreEqual(dt.Type(), DatumTypes::Pointer);
				Assert::AreEqual(r, dt.Get<RTTI*>());
				Assert::ExpectException<std::runtime_error>([&dt] {dt.Set(5); });
				Assert::ExpectException<std::out_of_range>([&dt, &r] {dt.Set(r, 10); });
			}
		}

	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState DatumTests::_startMemState;
}
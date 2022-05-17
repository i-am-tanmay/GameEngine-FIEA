#include "pch.h"
#include "AttributedFoo.h"
#include "AbsolutePerfectionAttributedFoo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(AttributedTests)
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

		TEST_METHOD(IsAttribute)
		{
			AttributedFoo foo;

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScope"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsAttribute("NOTATTRIBUTE"));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsPrescribedAttribute("NOTATTRIBUTE"));

			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NOTATTRIBUTE"));

			Assert::AreEqual(foo["this"].Get<RTTI*>()->As<AttributedFoo>(), &foo);
		}

		TEST_METHOD(Constructor)
		{
			AbsolutePerfectionAttributedFoo foo;

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScope"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foo.IsAttribute("NOTATTRIBUTE"));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foo.IsPrescribedAttribute("NOTATTRIBUTE"));

			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringPerfection"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorPerfection"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixPerfection"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArrayPerfection"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NOTATTRIBUTE"));

			Assert::AreEqual(foo["this"].Get<RTTI*>()->As<AbsolutePerfectionAttributedFoo>(), &foo);

			Assert::ExpectException<std::runtime_error>([] {AbsoluteGarbageAttributedFoo garbagefoo; });
		}

		TEST_METHOD(CopySymantics)
		{
			AbsolutePerfectionAttributedFoo foo;

			foo["ExternalInteger"] = 500;
			foo["ExternalFloat"] = 200.f;

			AbsolutePerfectionAttributedFoo foocopy = foo;

			Assert::AreEqual(foocopy["ExternalInteger"].Get<std::int32_t>(), std::int32_t(500));
			Assert::AreEqual(foocopy["ExternalFloat"].Get<float>(), 200.f);
			Assert::AreSame(foocopy["ExternalInteger"].Get<std::int32_t>(), foocopy.ExternalInteger);
			Assert::AreSame(foocopy["ExternalFloat"].Get<float>(), foocopy.ExternalFloat);

			Assert::IsTrue(foocopy.IsAttribute("this"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalString"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalVector"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foocopy.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foocopy.IsAttribute("NestedScope"));
			Assert::IsTrue(foocopy.IsAttribute("NestedScopeArray"));
			Assert::IsTrue(foocopy.IsAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foocopy.IsAttribute("NOTATTRIBUTE"));

			Assert::IsTrue(foocopy.IsPrescribedAttribute("this"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsTrue(foocopy.IsPrescribedAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foocopy.IsPrescribedAttribute("NOTATTRIBUTE"));

			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalStringPerfection"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalVectorPerfection"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalMatrixPerfection"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalFloatArrayPerfection"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("NestedScopeArray"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foocopy.IsAuxiliaryAttribute("NOTATTRIBUTE"));

			Assert::AreEqual(foocopy["this"].Get<RTTI*>()->As<AbsolutePerfectionAttributedFoo>(), &foocopy);

			AbsolutePerfectionAttributedFoo foocopyassignment;

			Assert::AreNotEqual(foocopyassignment["ExternalInteger"].Get<std::int32_t>(), std::int32_t(500));
			Assert::AreNotEqual(foocopyassignment["ExternalFloat"].Get<float>(), 200.f);

			foocopyassignment = foocopy;

			Assert::AreEqual(foocopyassignment["ExternalInteger"].Get<std::int32_t>(), std::int32_t(500));
			Assert::AreEqual(foocopyassignment["ExternalFloat"].Get<float>(), 200.f);
			Assert::AreSame(foocopyassignment["ExternalInteger"].Get<std::int32_t>(), foocopyassignment.ExternalInteger);
			Assert::AreSame(foocopyassignment["ExternalFloat"].Get<float>(), foocopyassignment.ExternalFloat);

			Assert::IsTrue(foocopyassignment.IsAttribute("this"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalString"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalVector"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foocopyassignment.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foocopyassignment.IsAttribute("NestedScope"));
			Assert::IsTrue(foocopyassignment.IsAttribute("NestedScopeArray"));
			Assert::IsTrue(foocopyassignment.IsAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foocopyassignment.IsAttribute("NOTATTRIBUTE"));

			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("this"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsTrue(foocopyassignment.IsPrescribedAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foocopyassignment.IsPrescribedAttribute("NOTATTRIBUTE"));

			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalStringPerfection"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalVectorPerfection"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalMatrixPerfection"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalFloatArrayPerfection"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("NestedScopeArray"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foocopyassignment.IsAuxiliaryAttribute("NOTATTRIBUTE"));

			Assert::AreEqual(foocopyassignment["this"].Get<RTTI*>()->As<AbsolutePerfectionAttributedFoo>(), &foocopyassignment);
		}

		TEST_METHOD(MoveSymantics)
		{
			AbsolutePerfectionAttributedFoo foo;

			foo["ExternalInteger"] = 500;
			foo["ExternalFloat"] = 200.f;

			AbsolutePerfectionAttributedFoo foomove = std::move(foo);

			Assert::AreEqual(foomove["ExternalInteger"].Get<std::int32_t>(), std::int32_t(500));
			Assert::AreEqual(foomove["ExternalFloat"].Get<float>(), 200.f);
			Assert::AreSame(foomove["ExternalInteger"].Get<std::int32_t>(), foomove.ExternalInteger);
			Assert::AreSame(foomove["ExternalFloat"].Get<float>(), foomove.ExternalFloat);

			Assert::IsTrue(foomove.IsAttribute("this"));
			Assert::IsTrue(foomove.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foomove.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foomove.IsAttribute("ExternalString"));
			Assert::IsTrue(foomove.IsAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foomove.IsAttribute("ExternalVector"));
			Assert::IsTrue(foomove.IsAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foomove.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foomove.IsAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foomove.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foomove.IsAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foomove.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foomove.IsAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foomove.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foomove.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foomove.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foomove.IsAttribute("NestedScope"));
			Assert::IsTrue(foomove.IsAttribute("NestedScopeArray"));
			Assert::IsTrue(foomove.IsAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foomove.IsAttribute("NOTATTRIBUTE"));

			Assert::IsTrue(foomove.IsPrescribedAttribute("this"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsTrue(foomove.IsPrescribedAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foomove.IsPrescribedAttribute("NOTATTRIBUTE"));

			Assert::IsFalse(foomove.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalStringPerfection"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalVectorPerfection"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalMatrixPerfection"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalFloatArrayPerfection"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("NestedScopeArray"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foomove.IsAuxiliaryAttribute("NOTATTRIBUTE"));

			Assert::AreEqual(foomove["this"].Get<RTTI*>()->As<AbsolutePerfectionAttributedFoo>(), &foomove);

			AbsolutePerfectionAttributedFoo foomoveassignment;

			Assert::AreNotEqual(foomoveassignment["ExternalInteger"].Get<std::int32_t>(), std::int32_t(500));
			Assert::AreNotEqual(foomoveassignment["ExternalFloat"].Get<float>(), 200.f);

			foomoveassignment = std::move(foomove);

			Assert::AreEqual(foomoveassignment["ExternalInteger"].Get<std::int32_t>(), std::int32_t(500));
			Assert::AreEqual(foomoveassignment["ExternalFloat"].Get<float>(), 200.f);
			Assert::AreSame(foomoveassignment["ExternalInteger"].Get<std::int32_t>(), foomoveassignment.ExternalInteger);
			Assert::AreSame(foomoveassignment["ExternalFloat"].Get<float>(), foomoveassignment.ExternalFloat);

			Assert::IsTrue(foomoveassignment.IsAttribute("this"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalString"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalVector"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foomoveassignment.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foomoveassignment.IsAttribute("NestedScope"));
			Assert::IsTrue(foomoveassignment.IsAttribute("NestedScopeArray"));
			Assert::IsTrue(foomoveassignment.IsAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foomoveassignment.IsAttribute("NOTATTRIBUTE"));

			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("this"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalStringPerfection"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalVectorPerfection"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalMatrixPerfection"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalFloatArrayPerfection"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsTrue(foomoveassignment.IsPrescribedAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foomoveassignment.IsPrescribedAttribute("NOTATTRIBUTE"));

			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalStringPerfection"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalVectorPerfection"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalMatrixPerfection"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalIntegerArrayPerfection"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalFloatArrayPerfection"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("NestedScopeArray"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("NestedScopeArrayPerfection"));
			Assert::IsFalse(foomoveassignment.IsAuxiliaryAttribute("NOTATTRIBUTE"));

			Assert::AreEqual(foomoveassignment["this"].Get<RTTI*>()->As<AbsolutePerfectionAttributedFoo>(), &foomoveassignment);
		}

		TEST_METHOD(AppendAuxiliary)
		{
			AbsolutePerfectionAttributedFoo foo;

			Assert::IsFalse(foo.IsAttribute("auxxx"));
			Datum& datum = foo.AppendAuxiliaryAttribute("auxxx");
			Assert::IsTrue(foo.IsAttribute("auxxx"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("auxxx"));
			Assert::IsFalse(foo.IsPrescribedAttribute("auxxx"));
			datum = 7.f;
			Assert::AreEqual(foo["auxxx"].Get<float>(), 7.f);
			Assert::AreSame(foo["auxxx"], datum);

			Assert::ExpectException<std::runtime_error>([&foo] {foo.AppendAuxiliaryAttribute("ExternalInteger"); });
		}

		TEST_METHOD(CloneEquals)
		{
			{
				AttributedFoo foo;
				AttributedFoo& cloneFoo = *(foo.Clone());
				Assert::IsTrue(foo.Equals(&cloneFoo));
				Foo foobj{ 10 };
				RTTI* rtti = &foobj;
				Assert::IsFalse(foo.Equals(rtti));
				delete& cloneFoo;
			}

			{
				AbsolutePerfectionAttributedFoo foo;
				AbsolutePerfectionAttributedFoo& cloneFoo = *(foo.Clone());
				Assert::IsTrue(foo.Equals(&cloneFoo));
				Foo foobj{ 10 };
				RTTI* rtti = &foobj;
				Assert::IsFalse(foo.Equals(rtti));
				delete& cloneFoo;
			}
		}

		TEST_METHOD(RTTIAttribute)
		{
			AttributedFoo foo;

			RTTI* rtti = &foo;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), rtti->TypeIdInstance());

			Foo* foobj = rtti->As<Foo>();
			Assert::IsNull(foobj);

			AttributedFoo* fooAS = rtti->As<AttributedFoo>();
			Assert::IsNotNull(fooAS);
			Assert::AreEqual(&foo, fooAS);

			Attributed* fooAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fooAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&foo), fooAttributed);

			AttributedFoo otherFoo;
			Assert::IsTrue(rtti->Equals(&otherFoo));

			Foo anotherFoo(100);
			Assert::IsFalse(rtti->Equals(&anotherFoo));
		}

	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState AttributedTests::_startMemState;
}
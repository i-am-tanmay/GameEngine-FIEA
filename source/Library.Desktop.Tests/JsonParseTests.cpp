#include "pch.h"
#include "RTTI.h"
#include "JsonParseManager.h"
#include "JsonParserInteger.h"
#include "JsonParserString.h"
#include "JsonParserVec.h"
#include "JsonParserScope.h"
#include "AttributedFoo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(JsonParseTests)
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

		TEST_METHOD(scopetest)
		{
			{
				std::string filename = "../../../../source/Library.Desktop.Tests/scope.json";

				JsonParserScope::SharedData sharedData;
				JsonParserScope scopeParser;
				JsonParseManager parseManager(sharedData);
				parseManager.AddParser(scopeParser);

				AttributedFooFactory factory_AttributedFoo;

				parseManager.ParseFile(filename);

				Datum* datum = sharedData.Data[0]->Find("AHealth"s);
				Assert::AreEqual(datum->Get<std::int32_t>(0u), 100);
				Assert::AreEqual(datum->Get<std::int32_t>(1u), 2);
				Assert::AreEqual(datum->Get<std::int32_t>(2u), 90);
				datum = sharedData.Data[0]->Find("BTransform"s);
				Assert::AreEqual(datum->Get<glm::vec4>(), glm::vec4{ 1.0f, 23.4f, 98.1f, 0.0f });
				datum = sharedData.Data[0]->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(0u), 13.75f);
				Assert::AreEqual(datum->Get<float>(1u), 0.02f);
				datum = sharedData.Data[0]->Find("EMPTY"s);
				Assert::AreEqual(datum->Type(), DatumTypes::ScopeTable);
				Assert::AreEqual(datum->Size(), std::size_t(0));

				datum = sharedData.Data[0]->Find("CWeapon"s);
				Scope* scope = datum->Get<Scope*>();
				Assert::IsNotNull(scope);

				{
					AttributedFoo* afoo = scope->As<AttributedFoo>();
					Assert::IsNotNull(afoo);

					Assert::IsTrue(afoo->IsAttribute("this"));
					Assert::IsTrue(afoo->IsAttribute("ExternalInteger"));
					Assert::IsTrue(afoo->IsAttribute("ExternalFloat"));
					Assert::IsTrue(afoo->IsAttribute("ExternalString"));
					Assert::IsTrue(afoo->IsAttribute("ExternalVector"));
					Assert::IsTrue(afoo->IsAttribute("ExternalMatrix"));
					Assert::IsTrue(afoo->IsAttribute("ExternalIntegerArray"));
					Assert::IsTrue(afoo->IsAttribute("ExternalFloatArray"));
					Assert::IsTrue(afoo->IsAttribute("ExternalStringArray"));
					Assert::IsTrue(afoo->IsAttribute("ExternalMatrixArray"));
					Assert::IsTrue(afoo->IsAttribute("ExternalVectorArray"));
					Assert::IsTrue(afoo->IsAttribute("NestedScope"));
					Assert::IsTrue(afoo->IsAttribute("NestedScopeArray"));
					Assert::IsFalse(afoo->IsAttribute("NOTATTRIBUTE"));

					Assert::IsTrue(afoo->IsPrescribedAttribute("this"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalInteger"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalFloat"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalString"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalVector"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalMatrix"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalIntegerArray"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalFloatArray"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalStringArray"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalMatrixArray"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("ExternalVectorArray"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("NestedScope"));
					Assert::IsTrue(afoo->IsPrescribedAttribute("NestedScopeArray"));
					Assert::IsFalse(afoo->IsPrescribedAttribute("NOTATTRIBUTE"));

					Assert::IsFalse(afoo->IsAuxiliaryAttribute("this"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalInteger"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalFloat"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalString"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalVector"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalMatrix"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalIntegerArray"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalFloatArray"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalStringArray"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalMatrixArray"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("ExternalVectorArray"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("NestedScope"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("NestedScopeArray"));
					Assert::IsFalse(afoo->IsAuxiliaryAttribute("NOTATTRIBUTE"));

					datum = scope->Find("ExternalIntegerArray"s);
					Assert::IsTrue(datum->IsExternalStorage());
					Assert::AreEqual(datum->Size(), std::size_t(5u));
					Assert::AreEqual(datum->Get<std::int32_t>(0u), 90);
					Assert::AreEqual(datum->Get<std::int32_t>(1u), 20);
					Assert::AreEqual(datum->Get<std::int32_t>(2u), 110);
					Assert::AreEqual(datum->Get<std::int32_t>(3u), 190);
					Assert::AreEqual(datum->Get<std::int32_t>(4u), 200);
				}

				datum = scope->Find("ASelectedWeapon"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 3);
				scope = scope->Find("BMelee")->Get<Scope*>()->Find("ASword")->Get<Scope*>()->Find("AKatana")->Get<Scope*>();
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 1);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(), 100.59f);

				scope = sharedData.Data[0]->Find("CWeapon"s)->Get<Scope*>()->Find("BMelee")->Get<Scope*>()->Find("ASword")->Get<Scope*>()->Find("BRapier")->Get<Scope*>();
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 2);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(0u), 70.59f);
				Assert::AreEqual(datum->Get<float>(1u), 189.90f);
				Assert::AreEqual(datum->Get<float>(2u), 228.9f);

				scope = sharedData.Data[0]->Find("CWeapon"s)->Get<Scope*>()->Find("BMelee")->Get<Scope*>()->Find("BSpear")->Get<Scope*>()->Find("AJavelin")->Get<Scope*>();
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 3);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(), 270.06f);

				scope = sharedData.Data[0]->Find("CWeapon"s)->Get<Scope*>()->Find("BRanged")->Get<Scope*>()->Find("ABowArrow")->Get<Scope*>();
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 4);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(), 40.1f);

				scope = sharedData.Data[0]->Find("CWeapon"s)->Get<Scope*>()->Find("BRanged")->Get<Scope*>()->Find("AShotgun")->Get<Scope*>(0u);
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(0u), 5);
				Assert::AreEqual(datum->Get<std::int32_t>(1u), 6);
				Assert::AreEqual(datum->Get<std::int32_t>(2u), 9);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(0u), 700.59f);
				Assert::AreEqual(datum->Get<float>(1u), 198.11f);
				Assert::AreEqual(datum->Get<float>(2u), 22.0f);
				datum = scope->Find("EMPTY"s);
				Assert::AreEqual(datum->Type(), DatumTypes::Matrix);
				Assert::AreEqual(datum->Size(), std::size_t(0));
				scope = sharedData.Data[0]->Find("CWeapon"s)->Get<Scope*>()->Find("BRanged")->Get<Scope*>()->Find("AShotgun")->Get<Scope*>(1u);
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(0u), 4);
				Assert::AreEqual(datum->Get<std::int32_t>(1u), 1);
				Assert::AreEqual(datum->Get<std::int32_t>(2u), 7);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(), 1.59f);
				datum = scope->Find("EMPTY"s);
				Assert::AreEqual(datum->Type(), DatumTypes::String);
				Assert::AreEqual(datum->Size(), std::size_t(0));


				scope = sharedData.Data[0]->Find("CWeapon"s)->Get<Scope*>();
				datum = scope->Find("EMPTY"s);
				Assert::AreEqual(datum->Type(), DatumTypes::ScopeTable);
				Assert::AreEqual(datum->Size(), std::size_t(0));


				// SECOND OBJ

				datum = sharedData.Data[1]->Find("AHealthAGAIN"s);
				Assert::AreEqual(datum->Get<std::int32_t>(0u), 20);
				Assert::AreEqual(datum->Get<std::int32_t>(1u), 87);
				datum = sharedData.Data[1]->Find("BTransform"s);
				Assert::AreEqual(datum->Get<glm::vec4>(), glm::vec4{ 1.0f, 23.4f, 98.1f, 0.0f });
				datum = sharedData.Data[1]->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(0u), 13.75f);
				Assert::AreEqual(datum->Get<float>(1u), 0.02f);
				datum = sharedData.Data[1]->Find("EMPTY"s);
				Assert::AreEqual(datum->Type(), DatumTypes::ScopeTable);
				Assert::AreEqual(datum->Size(), std::size_t(0));

				datum = sharedData.Data[1]->Find("CWeaponTWO"s);
				scope = datum->Get<Scope*>();
				Assert::IsNotNull(scope);
				datum = scope->Find("ASelectedWeapon"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 3);
				scope = scope->Find("BMelee")->Get<Scope*>()->Find("ASword")->Get<Scope*>()->Find("AKatana")->Get<Scope*>();
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 1);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(), 100.59f);

				scope = sharedData.Data[1]->Find("CWeaponTWO"s)->Get<Scope*>()->Find("BMelee")->Get<Scope*>()->Find("ASword")->Get<Scope*>()->Find("BRapier")->Get<Scope*>();
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 2);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(0u), 70.59f);
				Assert::AreEqual(datum->Get<float>(1u), 189.90f);
				Assert::AreEqual(datum->Get<float>(2u), 228.9f);

				scope = sharedData.Data[1]->Find("CWeaponTWO"s)->Get<Scope*>()->Find("BMelee")->Get<Scope*>()->Find("BSpear")->Get<Scope*>()->Find("AJavelin")->Get<Scope*>();
				datum = scope->Find("AidA"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 3);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(0u), 2.8f);
				Assert::AreEqual(datum->Get<float>(1u), 90.2f);

				scope = sharedData.Data[1]->Find("CWeaponTWO"s)->Get<Scope*>()->Find("BRanged")->Get<Scope*>()->Find("ABowArrow")->Get<Scope*>();
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(), 4);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(), 40.1f);

				scope = sharedData.Data[1]->Find("CWeaponTWO"s)->Get<Scope*>()->Find("BRanged")->Get<Scope*>()->Find("AShotgun")->Get<Scope*>(0u);
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(0u), 5);
				Assert::AreEqual(datum->Get<std::int32_t>(1u), 6);
				Assert::AreEqual(datum->Get<std::int32_t>(2u), 9);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(0u), 700.59f);
				Assert::AreEqual(datum->Get<float>(1u), 198.11f);
				Assert::AreEqual(datum->Get<float>(2u), 22.0f);
				datum = scope->Find("EMPTY"s);
				Assert::AreEqual(datum->Type(), DatumTypes::Matrix);
				Assert::AreEqual(datum->Size(), std::size_t(0));
				scope = sharedData.Data[1]->Find("CWeaponTWO"s)->Get<Scope*>()->Find("BRanged")->Get<Scope*>()->Find("AShotgun")->Get<Scope*>(1u);
				datum = scope->Find("Aid"s);
				Assert::AreEqual(datum->Get<std::int32_t>(0u), 4);
				Assert::AreEqual(datum->Get<std::int32_t>(1u), 1);
				Assert::AreEqual(datum->Get<std::int32_t>(2u), 7);
				datum = scope->Find("Damage"s);
				Assert::AreEqual(datum->Get<float>(), 1.59f);
				datum = scope->Find("EMPTY"s);
				Assert::AreEqual(datum->Type(), DatumTypes::String);
				Assert::AreEqual(datum->Size(), std::size_t(0));


				scope = sharedData.Data[1]->Find("CWeaponTWO"s)->Get<Scope*>();
				datum = scope->Find("EMPTY"s);
				Assert::AreEqual(datum->Type(), DatumTypes::ScopeTable);
				Assert::AreEqual(datum->Size(), std::size_t(0));
			}

			{
				JsonParserInteger::SharedData sharedData;
				JsonParserScope scopeParser;

				Assert::IsFalse(scopeParser.StartHandler(sharedData, std::string{}, Json::Value{}));
				Assert::IsFalse(scopeParser.EndHandler(sharedData, std::string{}, Json::Value{}));

			}

			{
				JsonParserScope::SharedData sharedData;
				JsonParserScope::SharedData* sdClone = sharedData.Create();
				Assert::AreEqual(sdClone->TypeIdInstance(), JsonParserScope::SharedData::TypeIdClass());
				delete sdClone;

				JsonParserScope scopeparser;
				JsonParserScope* scopeparserclone = static_cast<JsonParserScope*>(scopeparser.Create());
				Assert::IsNotNull(scopeparserclone);
				Assert::AreEqual(scopeparserclone->TypeIdInstance(), JsonParserScope::TypeIdClass());
				delete scopeparserclone;
			}
		}

		TEST_METHOD(scope_rtti)
		{
			{
				JsonParserScope parser;
				RTTI* rtti = &parser;

				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserInteger::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserString::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserVec::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParserScope::TypeIdClass()));
				Assert::IsTrue(rtti->Is(IJsonParser::TypeIdClass()));
				Assert::AreEqual(JsonParserScope::TypeIdClass(), rtti->TypeIdInstance());

				Foo* foo = rtti->As<Foo>();
				Assert::IsNull(foo);

				JsonParserScope* parserptr = rtti->As<JsonParserScope>();
				Assert::IsNotNull(parserptr);
				Assert::AreEqual(&parser, parserptr);
			}

			{
				JsonParserScope::SharedData sharedData;
				RTTI* rtti = &sharedData;

				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserInteger::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserString::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserVec::TypeIdClass()));
				Assert::IsFalse(rtti->Is(IJsonParser::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParseManager::SharedData::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParserScope::SharedData::TypeIdClass()));
				Assert::AreEqual(JsonParserScope::SharedData::TypeIdClass(), rtti->TypeIdInstance());

				Foo* f = rtti->As<Foo>();
				Assert::IsNull(f);

				JsonParserScope::SharedData* sdPtr = rtti->As<JsonParserScope::SharedData>();
				Assert::IsNotNull(sdPtr);
				Assert::AreEqual(&sharedData, sdPtr);
			}
		}

		TEST_METHOD(manager_setshareddata)
		{
			JsonParserInteger::SharedData sharedDataInteger;
			JsonParserString::SharedData sharedDataString;

			JsonParseManager parseManager(sharedDataInteger);
			Assert::AreSame(*(static_cast<JsonParserInteger::SharedData*>(parseManager.GetSharedData())), sharedDataInteger);
			Assert::AreSame(*(sharedDataInteger.GetJsonParseManager()), parseManager);
			const JsonParserInteger::SharedData& cSharedDataInteger = sharedDataInteger;
			Assert::AreSame(*(cSharedDataInteger.GetJsonParseManager()), parseManager);

			parseManager.SetSharedData(sharedDataString);
			Assert::AreSame(*(static_cast<JsonParserString::SharedData*>(parseManager.GetSharedData())), sharedDataString);
			Assert::IsNull(sharedDataInteger.GetJsonParseManager());
			Assert::AreSame(*(sharedDataString.GetJsonParseManager()), parseManager);
			const JsonParserString::SharedData& cSharedDataString = sharedDataString;
			Assert::AreSame(*(cSharedDataString.GetJsonParseManager()), parseManager);
		}

		TEST_METHOD(manager_addremoveparsers)
		{
			JsonParserInteger::SharedData sharedData;
			JsonParserInteger intParser;
			JsonParserString stringParser;
			JsonParserVec vec4Parser;
			JsonParseManager parseManager(sharedData);

			Assert::IsTrue(parseManager.AddParser(intParser));
			Assert::IsTrue(parseManager.AddParser(stringParser));
			Assert::IsTrue(parseManager.AddParser(vec4Parser));
			Assert::IsFalse(parseManager.AddParser(intParser));

			Assert::IsTrue(parseManager.RemoveParser(intParser));
			Assert::IsTrue(parseManager.RemoveParser(stringParser));
			Assert::IsTrue(parseManager.RemoveParser(vec4Parser));
			Assert::IsFalse(parseManager.RemoveParser(intParser));
		}

		TEST_METHOD(manager_getparser)
		{
			std::string inputstring = R"({ "integer": [ 77, 30, 22 ] })";

			JsonParserInteger::SharedData sharedData;
			JsonParseManager parseManager(sharedData);

			JsonParserInteger intParser;
			JsonParserString stringParser;
			JsonParserVec vec4Parser;
			parseManager.AddParser(stringParser);
			parseManager.AddParser(vec4Parser);
			parseManager.AddParser(intParser);

			JsonParseManager* clonedParseManager = parseManager.Clone();

			Assert::AreEqual(clonedParseManager->GetParserCount(), std::size_t(3));
			Assert::AreEqual(clonedParseManager->GetParser(0u).TypeIdInstance(), stringParser.TypeIdInstance());
			Assert::AreEqual(clonedParseManager->GetParser(1u).TypeIdInstance(), vec4Parser.TypeIdInstance());
			Assert::AreEqual(clonedParseManager->GetParser(2u).TypeIdInstance(), intParser.TypeIdInstance());

			clonedParseManager->RemoveParser(clonedParseManager->GetParser(0u));

			Assert::AreEqual(clonedParseManager->GetParserCount(), std::size_t(2));
			Assert::AreNotEqual(clonedParseManager->GetParser(0u).TypeIdInstance(), stringParser.TypeIdInstance());
			Assert::AreEqual(clonedParseManager->GetParser(0u).TypeIdInstance(), vec4Parser.TypeIdInstance());
			Assert::AreEqual(clonedParseManager->GetParser(1u).TypeIdInstance(), intParser.TypeIdInstance());

			delete clonedParseManager;
		}

		TEST_METHOD(integertest)
		{
			{
				std::string inputstring = R"({ "integer": 77 })";

				JsonParserInteger::SharedData sharedData;
				JsonParserInteger intParser;
				JsonParseManager parseManager(sharedData);

				JsonParserString stringParser;
				JsonParserVec vec4Parser;
				parseManager.AddParser(stringParser);
				parseManager.AddParser(vec4Parser);
				parseManager.AddParser(intParser);

				Assert::AreEqual(sharedData.Depth(), std::size_t(0));
				Assert::AreEqual(sharedData._maxdepth, std::size_t(0));

				parseManager.Parse(inputstring);
				Assert::AreEqual(sharedData.Data[0], std::int32_t(77));

				Assert::AreEqual(sharedData.Depth(), std::size_t(0));
				Assert::AreEqual(sharedData._maxdepth, std::size_t(1));

				intParser.Initialize();
				sharedData.Initialize();

				inputstring = R"({ "integer": [ 77, 30, 22 ] })";

				Assert::AreEqual(sharedData.Depth(), std::size_t(0));
				Assert::AreEqual(sharedData._maxdepth, std::size_t(0));

				parseManager.Parse(inputstring);
				Assert::AreEqual(sharedData.Data[0], std::int32_t(77));
				Assert::AreEqual(sharedData.Data[1], std::int32_t(30));
				Assert::AreEqual(sharedData.Data[2], std::int32_t(22));

				Assert::AreEqual(sharedData.Depth(), std::size_t(0));
				Assert::AreEqual(sharedData._maxdepth, std::size_t(1));
			}

			{
				std::string filename = "../../../../source/Library.Desktop.Tests/integer.json";

				JsonParserInteger::SharedData sharedData;
				JsonParserInteger intParser;
				JsonParseManager parseManager(sharedData);
				parseManager.AddParser(intParser);

				Assert::AreEqual(sharedData.Depth(), std::size_t(0));
				Assert::AreEqual(sharedData._maxdepth, std::size_t(0));

				parseManager.ParseFile(filename);
				Assert::AreEqual(sharedData.Data[0], std::int32_t(100));

				Assert::AreEqual(sharedData.Depth(), std::size_t(0));
				Assert::AreEqual(sharedData._maxdepth, std::size_t(1));
			}

			{
				std::string inputstring = R"([{ "integer": 77 }, { "integer": 77 }, { "integer": [ 77, 30, 22 ] }])";

				JsonParserInteger::SharedData sharedData;
				JsonParserInteger intParser;
				JsonParseManager parseManager(sharedData);

				JsonParserString stringParser;
				JsonParserVec vec4Parser;
				parseManager.AddParser(stringParser);
				parseManager.AddParser(vec4Parser);
				parseManager.AddParser(intParser);

				Assert::AreEqual(sharedData.Depth(), std::size_t(0));
				Assert::AreEqual(sharedData._maxdepth, std::size_t(0));

				parseManager.Parse(inputstring);
				Assert::AreEqual(sharedData.Data[0], std::int32_t(77));
				Assert::AreEqual(sharedData.Data[1], std::int32_t(77));
				Assert::AreEqual(sharedData.Data[2], std::int32_t(77));
				Assert::AreEqual(sharedData.Data[3], std::int32_t(30));
				Assert::AreEqual(sharedData.Data[4], std::int32_t(22));

				Assert::AreEqual(sharedData.Depth(), std::size_t(0));
				Assert::AreEqual(sharedData._maxdepth, std::size_t(1));
			}

			{
				JsonParserInteger::SharedData sharedData;
				JsonParserInteger intParser;
				JsonParseManager parseManager(sharedData);
				parseManager.AddParser(intParser);

				intParser.StartHandler(sharedData, "integer", Json::Value{3});
				Assert::ExpectException<std::runtime_error>([&intParser, &sharedData] {intParser.StartHandler(sharedData, "integer", Json::Value{ 3 }); });
				Assert::IsFalse(intParser.EndHandler(sharedData, "failinteger", Json::Value{}));
			}
		}

		TEST_METHOD(integertest_clone)
		{
			{
				std::string inputstring = R"({ "integer": [ 77, 30, 22 ] })";

				JsonParserInteger::SharedData sharedData;
				JsonParserInteger intParser;
				JsonParseManager parseManager(sharedData);

				JsonParserString stringParser;
				JsonParserVec vec4Parser;
				parseManager.AddParser(stringParser);
				parseManager.AddParser(vec4Parser);
				parseManager.AddParser(intParser);
				parseManager.AddParser(intParser);

				JsonParseManager* clonedParseManager = parseManager.Clone();
				JsonParserInteger::SharedData* clonedSharedData = static_cast<JsonParserInteger::SharedData*>(clonedParseManager->GetSharedData());
				Assert::IsNotNull(clonedSharedData);
				Assert::IsFalse(clonedParseManager->AddParser(stringParser));

				Assert::AreEqual(clonedSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(clonedSharedData->_maxdepth, std::size_t(0));

				clonedParseManager->Parse(inputstring);
				Assert::AreEqual(clonedSharedData->Data[0], std::int32_t(77));
				Assert::AreEqual(clonedSharedData->Data[1], std::int32_t(30));
				Assert::AreEqual(clonedSharedData->Data[2], std::int32_t(22));

				Assert::AreEqual(clonedSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(clonedSharedData->_maxdepth, std::size_t(1));

				delete clonedParseManager;
			}

			{
				std::string inputstring = R"({ "integer": [ 77, 30, 22 ] })";

				JsonParserInteger::SharedData sharedData;
				JsonParserInteger intParser;
				JsonParseManager parseManager(sharedData);

				JsonParserString stringParser;
				JsonParserVec vec4Parser;
				parseManager.AddParser(stringParser);
				parseManager.AddParser(vec4Parser);
				parseManager.AddParser(intParser);
				parseManager.AddParser(intParser);

				JsonParseManager* clonedParseManager = parseManager.Clone();
				JsonParserInteger::SharedData* clonedSharedData = static_cast<JsonParserInteger::SharedData*>(clonedParseManager->GetSharedData());
				Assert::IsNotNull(clonedSharedData);

				JsonParseManager movedParseManager = std::move(*clonedParseManager);
				JsonParserInteger::SharedData* movedSharedData = static_cast<JsonParserInteger::SharedData*>(movedParseManager.GetSharedData());
				Assert::IsNotNull(movedSharedData);
				Assert::AreSame(*movedSharedData, *clonedSharedData);

				Assert::AreEqual(movedSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(movedSharedData->_maxdepth, std::size_t(0));

				movedParseManager.Parse(inputstring);
				Assert::AreEqual(movedSharedData->Data[0], std::int32_t(77));
				Assert::AreEqual(movedSharedData->Data[1], std::int32_t(30));
				Assert::AreEqual(movedSharedData->Data[2], std::int32_t(22));

				Assert::AreEqual(movedSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(movedSharedData->_maxdepth, std::size_t(1));

				delete clonedParseManager;
			}

			{
				std::string inputstring = R"({ "integer": [ 77, 30, 22 ] })";

				JsonParserInteger::SharedData sharedData;
				JsonParserInteger intParser;
				JsonParseManager parseManager(sharedData);

				JsonParserString stringParser;
				JsonParserVec vec4Parser;
				parseManager.AddParser(stringParser);
				parseManager.AddParser(vec4Parser);
				parseManager.AddParser(intParser);
				parseManager.AddParser(intParser);

				JsonParseManager* clonedParseManager = parseManager.Clone();
				JsonParserInteger::SharedData* clonedSharedData = static_cast<JsonParserInteger::SharedData*>(clonedParseManager->GetSharedData());
				Assert::IsNotNull(clonedSharedData);

				JsonParseManager* clonedParseManagerII = parseManager.Clone();
				JsonParserInteger::SharedData* clonedSharedDataII = static_cast<JsonParserInteger::SharedData*>(clonedParseManagerII->GetSharedData());
				Assert::IsNotNull(clonedSharedDataII);

				*clonedParseManagerII = std::move(*clonedParseManager);
				JsonParserInteger::SharedData* movedSharedData = static_cast<JsonParserInteger::SharedData*>(clonedParseManagerII->GetSharedData());
				Assert::IsNotNull(movedSharedData);
				Assert::AreSame(*movedSharedData, *clonedSharedData);

				Assert::AreEqual(movedSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(movedSharedData->_maxdepth, std::size_t(0));

				clonedParseManagerII->Parse(inputstring);
				Assert::AreEqual(movedSharedData->Data[0], std::int32_t(77));
				Assert::AreEqual(movedSharedData->Data[1], std::int32_t(30));
				Assert::AreEqual(movedSharedData->Data[2], std::int32_t(22));

				Assert::AreEqual(movedSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(movedSharedData->_maxdepth, std::size_t(1));

				delete clonedParseManager;
				delete clonedParseManagerII;
			}
		}

		TEST_METHOD(integertest_rtti)
		{
			{
				JsonParserInteger parser;
				RTTI* rtti = &parser;

				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserString::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserVec::TypeIdClass()));
				Assert::IsTrue(rtti->Is(IJsonParser::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParserInteger::TypeIdClass()));
				Assert::AreEqual(JsonParserInteger::TypeIdClass(), rtti->TypeIdInstance());

				Foo* foo = rtti->As<Foo>();
				Assert::IsNull(foo);

				JsonParserInteger* parserptr = rtti->As<JsonParserInteger>();
				Assert::IsNotNull(parserptr);
				Assert::AreEqual(&parser, parserptr);
			}

			{
				JsonParserInteger::SharedData sharedData;
				RTTI* rtti = &sharedData;

				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserInteger::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserString::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserVec::TypeIdClass()));
				Assert::IsFalse(rtti->Is(IJsonParser::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParseManager::SharedData::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParserInteger::SharedData::TypeIdClass()));
				Assert::AreEqual(JsonParserInteger::SharedData::TypeIdClass(), rtti->TypeIdInstance());

				Foo* f = rtti->As<Foo>();
				Assert::IsNull(f);

				JsonParserInteger::SharedData* sdPtr = rtti->As<JsonParserInteger::SharedData>();
				Assert::IsNotNull(sdPtr);
				Assert::AreEqual(&sharedData, sdPtr);
			}
		}

		TEST_METHOD(stringtest)
		{
			{
				std::string inputstring = R"({ "string": "heyy" })";

				JsonParserString::SharedData sharedData;
				JsonParserString::SharedData* hSharedData = sharedData.Create();
				Assert::IsNotNull(hSharedData);
				JsonParserString stringParser;
				JsonParserString* hStringParser = static_cast<JsonParserString*>(stringParser.Create());
				JsonParseManager parseManager(*hSharedData);
				parseManager.AddParser(*hStringParser);

				Assert::AreEqual(hSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(hSharedData->_maxdepth, std::size_t(0));

				parseManager.Parse(inputstring);
				Assert::AreEqual(hSharedData->Data[0], "heyy"s);

				Assert::AreEqual(hSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(hSharedData->_maxdepth, std::size_t(1));

				hStringParser->Initialize();
				hSharedData->Initialize();

				inputstring = R"({ "string": [ "hee", "ee", "eyyyy" ] })";

				Assert::AreEqual(hSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(hSharedData->_maxdepth, std::size_t(0));

				parseManager.Parse(inputstring);
				Assert::AreEqual(hSharedData->Data[0], "hee"s);
				Assert::AreEqual(hSharedData->Data[1], "ee"s);
				Assert::AreEqual(hSharedData->Data[2], "eyyyy"s);

				Assert::AreEqual(hSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(hSharedData->_maxdepth, std::size_t(1));

				delete hStringParser;
				delete hSharedData;
			}

			{
				JsonParserString::SharedData sharedData;
				JsonParserString stringParser;
				JsonParseManager parseManager(sharedData);
				parseManager.AddParser(stringParser);

				stringParser.StartHandler(sharedData, "string", Json::Value{"abc"});
				Assert::ExpectException<std::runtime_error>([&stringParser, &sharedData] {stringParser.StartHandler(sharedData, "string", Json::Value{ "abc" }); });
				Assert::IsFalse(stringParser.EndHandler(sharedData, "failstring", Json::Value{}));
			}
		}

		TEST_METHOD(stringtest_rtti)
		{
			{
				JsonParserString parser;
				RTTI* rtti = &parser;

				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserInteger::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserVec::TypeIdClass()));
				Assert::IsTrue(rtti->Is(IJsonParser::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParserString::TypeIdClass()));
				Assert::AreEqual(JsonParserString::TypeIdClass(), rtti->TypeIdInstance());

				Foo* foo = rtti->As<Foo>();
				Assert::IsNull(foo);

				JsonParserString* parserptr = rtti->As<JsonParserString>();
				Assert::IsNotNull(parserptr);
				Assert::AreEqual(&parser, parserptr);
			}

			{
				JsonParserString::SharedData sharedData;
				RTTI* rtti = &sharedData;

				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserInteger::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserString::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserVec::TypeIdClass()));
				Assert::IsFalse(rtti->Is(IJsonParser::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParseManager::SharedData::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParserString::SharedData::TypeIdClass()));
				Assert::AreEqual(JsonParserString::SharedData::TypeIdClass(), rtti->TypeIdInstance());

				Foo* f = rtti->As<Foo>();
				Assert::IsNull(f);

				JsonParserString::SharedData* sdPtr = rtti->As<JsonParserString::SharedData>();
				Assert::IsNotNull(sdPtr);
				Assert::AreEqual(&sharedData, sdPtr);
			}
		}

		TEST_METHOD(vectest)
		{
			{
				std::string inputstring = "{ \"vec4\": \"vec4(1.0, 2.1, 6.4, 99.2)\" }";

				JsonParserVec::SharedData sharedData;
				JsonParserVec::SharedData* hSharedData = sharedData.Create();
				Assert::IsNotNull(hSharedData);
				JsonParserVec vec4Parser;
				JsonParserVec* hVec4Parser = static_cast<JsonParserVec*>(vec4Parser.Create());
				JsonParseManager parseManager(*hSharedData);
				parseManager.AddParser(*hVec4Parser);

				Assert::AreEqual(hSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(hSharedData->_maxdepth, std::size_t(0));

				parseManager.Parse(inputstring);
				Assert::AreEqual(hSharedData->Data[0], glm::vec4(1.f, 2.1f, 6.4f, 99.2f));

				Assert::AreEqual(hSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(hSharedData->_maxdepth, std::size_t(1));

				hVec4Parser->Initialize();
				hSharedData->Initialize();

				inputstring = "{ \"vec4\": [\"vec4(1.0, 2.1, 6.4, 99.2)\", \"vec4(34.0, 11.1, 64.44, 919.22)\", \"vec4(0.12, 22.12, 56.54, 12.23)\"] }";

				Assert::AreEqual(hSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(hSharedData->_maxdepth, std::size_t(0));

				parseManager.Parse(inputstring);
				Assert::AreEqual(hSharedData->Data[0], glm::vec4(1.f, 2.1f, 6.4f, 99.2f));
				Assert::AreEqual(hSharedData->Data[1], glm::vec4(34.0f, 11.1f, 64.44f, 919.22f));
				Assert::AreEqual(hSharedData->Data[2], glm::vec4(0.12f, 22.12f, 56.54f, 12.23f));

				Assert::AreEqual(hSharedData->Depth(), std::size_t(0));
				Assert::AreEqual(hSharedData->_maxdepth, std::size_t(1));

				delete hVec4Parser;
				delete hSharedData;
			}

			{
				JsonParserVec::SharedData sharedData;
				JsonParserVec vecParser;
				JsonParseManager parseManager(sharedData);
				parseManager.AddParser(vecParser);

				vecParser.StartHandler(sharedData, "vec4", Json::Value{ "abc" });
				Assert::ExpectException<std::runtime_error>([&vecParser, &sharedData] {vecParser.StartHandler(sharedData, "vec4", Json::Value{ "abc" }); });
				Assert::IsFalse(vecParser.EndHandler(sharedData, "failvec4", Json::Value{}));
			}
		}

		TEST_METHOD(vectest_rtti)
		{
			{
				JsonParserVec parser;
				RTTI* rtti = &parser;

				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserInteger::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserString::TypeIdClass()));
				Assert::IsTrue(rtti->Is(IJsonParser::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParserVec::TypeIdClass()));
				Assert::AreEqual(JsonParserVec::TypeIdClass(), rtti->TypeIdInstance());

				Foo* foo = rtti->As<Foo>();
				Assert::IsNull(foo);

				JsonParserVec* parserptr = rtti->As<JsonParserVec>();
				Assert::IsNotNull(parserptr);
				Assert::AreEqual(&parser, parserptr);
			}

			{
				JsonParserVec::SharedData sharedData;
				RTTI* rtti = &sharedData;

				Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserString::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserVec::TypeIdClass()));
				Assert::IsFalse(rtti->Is(IJsonParser::TypeIdClass()));
				Assert::IsFalse(rtti->Is(JsonParserInteger::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParseManager::SharedData::TypeIdClass()));
				Assert::IsTrue(rtti->Is(JsonParserVec::SharedData::TypeIdClass()));
				Assert::AreEqual(JsonParserVec::SharedData::TypeIdClass(), rtti->TypeIdInstance());

				Foo* f = rtti->As<Foo>();
				Assert::IsNull(f);

				JsonParserVec::SharedData* sdPtr = rtti->As<JsonParserVec::SharedData>();
				Assert::IsNotNull(sdPtr);
				Assert::AreEqual(&sharedData, sdPtr);
			}
		}

	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState JsonParseTests::_startMemState;
}
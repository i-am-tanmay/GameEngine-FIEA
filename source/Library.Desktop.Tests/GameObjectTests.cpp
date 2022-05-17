#include "pch.h"
#include "RTTI.h"
#include "JsonParseManager.h"
#include "JsonParserScope.h"
#include "GameObject.h"
#include "GOFoo.h"
#include "ToStringSpecializations.h"
#include "GameTime.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(GameObjectTests)
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

		TEST_METHOD(gameobject_parse)
		{
			{
				std::string filename = "../../../../source/Library.Desktop.Tests/gameobject.json";

				JsonParserScope::SharedData sharedData;
				JsonParserScope scopeParser;
				JsonParseManager parseManager(sharedData);
				parseManager.AddParser(scopeParser);

				GameObjectFactory gofactory;
				GOFooFactory gfoofactory;
				parseManager.ParseFile(filename);

				float testvalue = 0.f;

				Datum* datum = sharedData.Data[0]->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "root_scene"s);

				datum = sharedData.Data[0]->Find("Transform"s)->Get<Scope*>(0u)->Find("Position"s);
				Assert::AreEqual(datum->Get<glm::vec4>(0u), glm::vec4{ 1.0,5.0,7.0,4.0 });
				datum = sharedData.Data[0]->Find("Transform"s)->Get<Scope*>(0u)->Find("Rotation"s);
				Assert::AreEqual(datum->Get<glm::vec4>(0u), glm::vec4{ 11.0,15.0,17.0,14.0 });
				datum = sharedData.Data[0]->Find("Transform"s)->Get<Scope*>(0u)->Find("Scale"s);
				Assert::AreEqual(datum->Get<glm::vec4>(0u), glm::vec4{ 21.0,25.0,27.0,24.0 });
				Scope* scope = sharedData.Data[0].get();
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Name"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Children"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Transform"));

				datum = sharedData.Data[0]->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Name"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Children"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Transform"));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c1_0"s);
				datum = scope->Find("Transform"s)->Get<Scope*>(0u)->Find("Position"s);
				Assert::AreEqual(datum->Get<glm::vec4>(0u), glm::vec4{ 31.0, 35.0, 37.0, 34.0 });
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(0u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(1u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Name"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Children"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Transform"));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c1_1"s);
				datum = scope->Find("Transform"s)->Get<Scope*>(0u)->Find("Rotation"s);
				Assert::AreEqual(datum->Get<glm::vec4>(0u), glm::vec4{ 51.0,55.0,57.0,54.0 });

				datum = sharedData.Data[0]->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Name"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Children"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Transform"));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_0"s);
				datum = scope->Find("Transform"s)->Get<Scope*>(0u)->Find("Scale"s);
				Assert::AreEqual(datum->Get<glm::vec4>(0u), glm::vec4{ 41.0,45.0,47.0,44.0 });

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Name"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Children"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Transform"));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_1"s);
				datum = scope->Find("Transform"s)->Get<Scope*>(0u)->Find("Rotation"s);
				Assert::AreEqual(datum->Get<glm::vec4>(0u), glm::vec4{ 61.0,65.0,67.0,64.0 });

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Name"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Children"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Transform"));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_2"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);
				datum = scope->Find("Transform"s)->Get<Scope*>(0u)->Find("Position"s);
				Assert::AreEqual(datum->Get<glm::vec4>(0u), glm::vec4{ 71.0,75.0,77.0,74.0 });

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(1u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Name"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Children"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Transform"));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_2_0"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);
				datum = scope->Find("Transform"s)->Get<Scope*>(0u)->Find("Scale"s);
				Assert::AreEqual(datum->Get<glm::vec4>(0u), glm::vec4{ 71.0,75.0,77.0,74.0 });

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsFalse(scope->Is(Attributed::TypeIdClass()));
				Assert::IsFalse(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_0"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Name"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Children"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Transform"));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_1"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), 0.f);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(2u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Name"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Children"));
				Assert::IsTrue(static_cast<Attributed*>(scope)->IsPrescribedAttribute("Transform"));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_2"s);

				//////////////////////////// CHECK IF IT UPDATES ALL CHILDREN ////////////////////////////
				GameTime time;
				sharedData.Data[0]->Update(time);
				testvalue += 0.5f;

				datum = sharedData.Data[0]->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "root_scene"s);

				datum = sharedData.Data[0]->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c1_0"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(0u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(1u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c1_1"s);

				datum = sharedData.Data[0]->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_0"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_1"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_2"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(1u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_2_0"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsFalse(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_0"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_1"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), 0.f);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(2u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_2"s);

				//////////////////////////// CHECK IF IT UPDATES ALL CHILDREN ////////////////////////////
				sharedData.Data[0]->Update(time);
				testvalue += 0.5f;

				datum = sharedData.Data[0]->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "root_scene"s);

				datum = sharedData.Data[0]->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c1_0"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(0u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(1u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c1_1"s);

				datum = sharedData.Data[0]->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_0"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_1"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_2"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(1u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_2_0"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsFalse(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_0"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_1"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), 0.f);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(2u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_2"s);

				//////////////////////////// CHECK IF IT UPDATES ALL CHILDREN ////////////////////////////
				sharedData.Data[0]->Update(time);
				testvalue += 0.5f;

				datum = sharedData.Data[0]->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "root_scene"s);

				datum = sharedData.Data[0]->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c1_0"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(0u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(1u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c1_1"s);

				datum = sharedData.Data[0]->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_0"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_1"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(2u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_2"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s);
				Assert::AreEqual(datum->Size(), std::size_t(1u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "c2_2_0"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), testvalue);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(0u);
				Assert::IsFalse(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_0"s);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(1u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_1"s);
				datum = scope->Find("testfloat"s);
				Assert::AreEqual(datum->Get<float>(0u), 0.f);

				datum = sharedData.Data[0]->Find("Children"s)->Get<Scope*>(1u)->Find("Children"s)->Get<Scope*>(1u)->Find("NOTCHILDREN"s);
				Assert::AreEqual(datum->Size(), std::size_t(3u));
				scope = datum->Get<Scope*>(2u);
				Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
				Assert::IsFalse(scope->Is(GOFoo::TypeIdClass()));
				datum = scope->Find("Name"s);
				Assert::AreEqual(datum->Get<std::string>(0u), "nc2_2_2"s);
			}

			{
				GameObject gameobject;
				GameObject& gameobjectclone = *(gameobject.Clone());
				Assert::IsTrue(gameobject.Equals(&gameobjectclone));
				Assert::IsTrue(gameobjectclone.Is(GameObject::TypeIdClass()));
				Assert::AreEqual(gameobjectclone.ToString(), gameobjectclone.Name);
				delete& gameobjectclone;
			}

			{
				GOFoo gofoo;
				GOFoo& gofooclone = *(gofoo.Clone());
				Assert::IsTrue(gofoo.Equals(&gofooclone));
				Assert::IsTrue(gofoo.Is(GOFoo::TypeIdClass()));
				Assert::AreEqual(gofooclone.ToString(), gofooclone.Name);
				delete& gofooclone;
			}
		}

		TEST_METHOD(gameobject_rtti)
		{
			GameObject gameobject;

			RTTI* rtti = &gameobject;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(GameObject::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(GameObject::TypeIdClass(), rtti->TypeIdInstance());

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			GameObject* f = rtti->As<GameObject>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&gameobject, f);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&gameobject), fAsAttributed);

			GameObject otherObject;
			Assert::IsTrue(rtti->Equals(&otherObject));

			RTTI* nullPtr = nullptr;
			Assert::IsFalse(gameobject.Equals(nullPtr));
		}

	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState GameObjectTests::_startMemState;
}
#include "pch.h"
#include "ActionList_IF.h"
#include "Action_CreateAction.h"
#include "Action_DestroyAction.h"
#include "ActionManager.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(ActionTests)
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


		TEST_METHOD(action_manager)
		{
			std::string filename = "../../../../source/Library.Desktop.Tests/action.json";

			JsonParserScope::SharedData sharedData;
			JsonParserScope scopeParser;
			JsonParseManager parseManager(sharedData);
			parseManager.AddParser(scopeParser);

			GameObjectFactory gofactory;
			ActionListFactory actionlistfactory;
			ActionList_IFFactory iffactory;
			Action_CreateActionFactory createfactory;
			Action_DestroyActionFactory destroyfactory;
			parseManager.ParseFile(filename);

			GameTime gametime;

			Datum* datum = sharedData.Data[0]->Find("Name"s);
			Assert::AreEqual(datum->Get<std::string>(0u), "ROOT"s);
			datum = sharedData.Data[0]->Find("Actions"s);
			Assert::AreEqual(datum->Size(), std::size_t(2u));
			Datum* child_actions = sharedData.Data[0]->Find("Children"s)->Get<Scope*>()->Find("Actions");
			Scope* scope = datum->Get<Scope*>();
			sharedData.Data[0]->Update(gametime);
			Assert::AreEqual(datum->Size(), std::size_t(2u));
			Assert::AreEqual(child_actions->Size(), std::size_t(0u));
			ActionManager::Update();
			Assert::AreEqual(datum->Size(), std::size_t(3u));
			Assert::AreEqual(datum->Get<Scope*>(0u)->Find("Name"s)->Get<std::string>(), "IFCHECK_1"s);
			Assert::AreEqual(datum->Get<Scope*>(1u)->Find("Name"s)->Get<std::string>(), "IFCHECK_2"s);
			Assert::AreEqual(datum->Get<Scope*>(2u)->Find("Name"s)->Get<std::string>(), "CREATE 1"s);
			Assert::AreEqual(child_actions->Size(), std::size_t(1u));
			Assert::AreEqual(child_actions->Get<Scope*>()->Find("Name"s)->Get<std::string>(), "CREATE 2"s);

			datum->Get<Scope*>(1u)->Find("Actions")->Get<Scope*>()->Find("CreateContext")->Set("CREATE 2"s);
			sharedData.Data[0]->Update(gametime);
			Assert::AreEqual(datum->Size(), std::size_t(3u));
			Assert::AreEqual(child_actions->Size(), std::size_t(1u));
			Assert::AreEqual(child_actions->Get<Scope*>()->Find("Actions"s)->Size(), std::size_t(0u));
			ActionManager::Update();
			Assert::AreEqual(datum->Size(), std::size_t(4u));
			Assert::AreEqual(datum->Get<Scope*>(0u)->Find("Name"s)->Get<std::string>(), "IFCHECK_1"s);
			Assert::AreEqual(datum->Get<Scope*>(1u)->Find("Name"s)->Get<std::string>(), "IFCHECK_2"s);
			Assert::AreEqual(datum->Get<Scope*>(2u)->Find("Name"s)->Get<std::string>(), "CREATE 1"s);
			Assert::AreEqual(datum->Get<Scope*>(3u)->Find("Name"s)->Get<std::string>(), "CREATE 1"s);
			Assert::AreEqual(child_actions->Size(), std::size_t(1u));
			Assert::AreEqual(child_actions->Get<Scope*>()->Find("Name"s)->Get<std::string>(), "CREATE 2"s);
			Assert::AreEqual(child_actions->Get<Scope*>()->Find("Actions"s)->Size(), std::size_t(1u));
			Assert::AreEqual(child_actions->Get<Scope*>()->Find("Actions"s)->Get<Scope*>()->Find("Name"s)->Get<std::string>(), "CREATE 2"s);

			scope->Find("condition")->Set(1);
			datum->Get<Scope*>(1u)->Find("condition")->Set(1);
			sharedData.Data[0]->Update(gametime);
			Assert::AreEqual(datum->Size(), std::size_t(4u));
			Assert::AreEqual(child_actions->Size(), std::size_t(1u));
			ActionManager::Update();
			Assert::AreEqual(datum->Size(), std::size_t(2u));
			Assert::AreEqual(datum->Get<Scope*>(0u)->Find("Name"s)->Get<std::string>(), "CREATE 1"s);
			Assert::AreEqual(datum->Get<Scope*>(1u)->Find("Name"s)->Get<std::string>(), "CREATE 1"s);
			Assert::AreEqual(child_actions->Size(), std::size_t(1u));
			Assert::AreEqual(child_actions->Get<Scope*>()->Find("Name"s)->Get<std::string>(), "CREATE 2"s);
			Assert::AreEqual(child_actions->Get<Scope*>()->Find("Actions"s)->Size(), std::size_t(1u));
			Assert::AreEqual(child_actions->Get<Scope*>()->Find("Actions"s)->Get<Scope*>()->Find("Name"s)->Get<std::string>(), "CREATE 2"s);

			ActionManager::CleanUp();
		}

		TEST_METHOD(create_rtti)
		{
			Action_CreateAction action;

			RTTI* rtti = &action;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Action_CreateAction::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Action_CreateAction::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			Action_CreateAction* a = rtti->As<Action_CreateAction>();
			Assert::IsNotNull(a);
			Assert::AreEqual(&action, a);

			Assert::IsTrue(action.Equals(rtti));
			Assert::IsFalse(action.Equals(nullptr));

			Action* clone = action.Clone();
			Assert::AreEqual(clone->ToString(), action.Name);
			delete clone;
		}

		TEST_METHOD(destroy_rtti)
		{
			Action_DestroyAction action;

			RTTI* rtti = &action;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Action_DestroyAction::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Action_DestroyAction::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			Action_DestroyAction* a = rtti->As<Action_DestroyAction>();
			Assert::IsNotNull(a);
			Assert::AreEqual(&action, a);

			Assert::IsTrue(action.Equals(rtti));
			Assert::IsFalse(action.Equals(nullptr));

			Action* clone = action.Clone();
			Assert::AreEqual(clone->ToString(), action.Name);
			delete clone;
		}

		TEST_METHOD(list_rtti)
		{
			ActionList action;

			RTTI* rtti = &action;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionList::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionList::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			ActionList* a = rtti->As<ActionList>();
			Assert::IsNotNull(a);
			Assert::AreEqual(&action, a);

			Assert::IsTrue(action.Equals(rtti));
			Assert::IsFalse(action.Equals(nullptr));

			Action* clone = action.Clone();
			Assert::AreEqual(clone->ToString(), action.Name);
			delete clone;
		}

		TEST_METHOD(if_rtti)
		{
			ActionList_IF action;

			RTTI* rtti = &action;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionList_IF::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionList::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionList_IF::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			ActionList_IF* a = rtti->As<ActionList_IF>();
			Assert::IsNotNull(a);
			Assert::AreEqual(&action, a);

			Assert::IsTrue(action.Equals(rtti));
			Assert::IsFalse(action.Equals(nullptr));

			Action* clone = action.Clone();
			Assert::AreEqual(clone->ToString(), action.Name);
			delete clone;
		}


	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState ActionTests::_startMemState;
}
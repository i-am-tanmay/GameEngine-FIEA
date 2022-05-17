#include "pch.h"
#include "AttributedFoo.h"
#include "AbsolutePerfectionAttributedFoo.h"
#include "GameObject.h"
#include "GOFoo.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionList_IF.h"
#include "Action_CreateAction.h"
#include "Action_DestroyAction.h"
#include "TypeManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		RegisterSingleType(Attributed);

		RegisterType(AttributedFoo, Attributed);
		RegisterType(AbsolutePerfectionAttributedFoo, AttributedFoo);
		RegisterType(AbsoluteGarbageAttributedFoo, AttributedFoo);

		RegisterType(Transform, Attributed);
		RegisterType(GameObject, Attributed);
		RegisterType(GOFoo, GameObject);

		RegisterType(Action, Attributed);
		RegisterType(ActionList, Action);
		RegisterType(ActionList_IF, ActionList);
		RegisterType(Action_CreateAction, Action);
		RegisterType(Action_DestroyAction, Action);
	}

	TEST_MODULE_CLEANUP(TestModuleCleanup)
	{
		TypeManager::GetInstance().Clear();
	}
}
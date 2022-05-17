#include "pch.h"
#include "Foo.h"
#include "Scope.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(ScopeTests)
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
			Scope root;
			Assert::IsNull(root.GetParent());
		}

		TEST_METHOD(CopySymantics)
		{
			{
				Scope root;
				Scope child;
				Scope child2;
				root.AppendScope("c1") = child;
				root.Append("c2") = 3;
				root.Adopt(child2, "c3");
				root.Append("c4") = glm::mat4(29.f);

				Scope root_copy = root;
				Assert::AreEqual(root, root_copy);

				Scope root_copyagain;
				root_copyagain = root_copy;
				Assert::AreEqual(root_copyagain, root_copy);
				Assert::AreEqual(root_copyagain, root);
			}

			{
				Scope root;
				root.Append("c0");
				Scope& child_root = root.AppendScope("c1");

				Scope* child = new Scope();
				Scope* child2 = new Scope();
				child_root.AppendScope("c1") = *child;
				child_root.Append("c2") = 3;
				child_root.Adopt(*child2, "c3");
				child_root.Append("c4") = glm::mat4(29.f);

				Scope root_copy = child_root;
				Assert::AreEqual(child_root, root_copy);

				Scope root_copyagain;
				root_copyagain = root_copy;
				Assert::AreEqual(root_copyagain, root_copy);
				Assert::AreEqual(root_copyagain, child_root);

				delete child;
			}
		}

		TEST_METHOD(MoveSymantics)
		{
			{
				Scope root;
				Scope& child = root.AppendScope("c1");
				Scope* child2 = new Scope();
				root.Append("c2") = 3;
				root.Adopt(*child2, "c3");
				root.Append("c4") = glm::mat4(29.f);

				Scope root_move = std::move(root);
				Assert::AreEqual((*root_move.Find("c1"))[0], child);
				Assert::AreEqual(*root_move.Find("c2"), Datum{ 3 });
				Assert::AreEqual((*root_move.Find("c3"))[0], *child2);
				Assert::AreEqual(*root_move.Find("c4"), Datum{ glm::mat4{29.f } });
				Assert::AreEqual(child.GetParent(), &root_move);
				Assert::AreEqual(child2->GetParent(), &root_move);

				Scope root_moveagain;
				root_moveagain = std::move(root_move);
				Assert::AreEqual((*root_moveagain.Find("c1"))[0], child);
				Assert::AreEqual(*root_moveagain.Find("c2"), Datum{ 3 });
				Assert::AreEqual((*root_moveagain.Find("c3"))[0], *child2);
				Assert::AreEqual(*root_moveagain.Find("c4"), Datum{ glm::mat4{29.f } });
				Assert::AreEqual(child.GetParent(), &root_moveagain);
				Assert::AreEqual(child2->GetParent(), &root_moveagain);
			}

			{
				Scope root;
				root.Append("c0");
				Scope* child_root = new Scope{};
				root.Adopt(*child_root, "c1");

				Scope& child = child_root->AppendScope("c1");
				Scope* child2 = new Scope();
				child_root->Append("c2") = 3;
				child_root->Adopt(*child2, "c3");
				child_root->Append("c4") = glm::mat4(29.f);

				Scope root_move = std::move(*child_root);
				Assert::AreEqual((root.FindContainedScope(root_move).first)->Get<Scope*>(), &root_move);
				Assert::IsNull(root.FindContainedScope(*child_root).first);
				Assert::AreEqual((*root_move.Find("c1"))[0], child);
				Assert::AreEqual(*root_move.Find("c2"), Datum{ 3 });
				Assert::AreEqual((*root_move.Find("c3"))[0], *child2);
				Assert::AreEqual(*root_move.Find("c4"), Datum{ glm::mat4{29.f } });
				Assert::AreEqual(child.GetParent(), &root_move);
				Assert::AreEqual(child2->GetParent(), &root_move);
				delete child_root;

				Scope root_moveagain;
				root_moveagain = std::move(root_move);
				Assert::AreEqual((root.FindContainedScope(root_moveagain).first)->Get<Scope*>(), &root_moveagain);
				Assert::IsNull(root.FindContainedScope(root_move).first);
				Assert::AreEqual((*root_moveagain.Find("c1"))[0], child);
				Assert::AreEqual(*root_moveagain.Find("c2"), Datum{ 3 });
				Assert::AreEqual((*root_moveagain.Find("c3"))[0], *child2);
				Assert::AreEqual(*root_moveagain.Find("c4"), Datum{ glm::mat4{29.f } });
				Assert::AreEqual(child.GetParent(), &root_moveagain);
				Assert::AreEqual(child2->GetParent(), &root_moveagain);
			}
		}

		TEST_METHOD(Append)
		{
			Scope root;

			Assert::ExpectException<std::runtime_error>([&root] {root.Append(""); });

			Datum& dtmunknown = root.Append("c1");
			Assert::AreEqual(dtmunknown.Type(), DatumTypes::Unknown);
			Assert::AreEqual(*root.Find("c1"), dtmunknown);

			Datum& dtmint = root.Append("c2", DatumTypes::Integer);
			Assert::AreEqual(dtmint.Type(), DatumTypes::Integer);
			Assert::AreEqual(*root.Find("c2"), dtmint);

			root.Append("c2", DatumTypes::Float);
			Assert::AreEqual(dtmint.Type(), DatumTypes::Integer);
			Assert::AreEqual(*root.Find("c2"), dtmint);
		}

		TEST_METHOD(AppendScope)
		{
			Scope root;

			Assert::ExpectException<std::runtime_error>([&root] {root.AppendScope(""); });

			Scope& child1 = root.AppendScope("c1");
			Assert::AreEqual((*root.Find("c1"))[0], child1);
			Assert::AreEqual(child1.GetParent(), &root);
			Scope& child1_2 = root.AppendScope("c1");
			Assert::AreEqual((*root.Find("c1"))[0], child1);
			Assert::AreEqual((*root.Find("c1"))[1], child1_2);
			Assert::AreEqual(child1_2.GetParent(), &root);

			child1.Append("c1");
			Scope& child1_c1 = child1.AppendScope("c1");
			Assert::AreEqual((*child1.Find("c1"))[0], child1_c1);
			Assert::AreEqual(child1_c1.GetParent(), &child1);

			child1.Append("c2", DatumTypes::Integer);
			Assert::ExpectException<std::runtime_error>([&child1] {child1.AppendScope("c2"); });

			int x;
			child1["cext"].SetStorageExternal(&x, 2);
			Assert::ExpectException<std::runtime_error>([&child1] {child1.AppendScope("cext"); });
		}

		TEST_METHOD(operatorBracket)
		{
			Scope root;

			Datum& child1 = root["c1"];
			Assert::AreEqual(*root.Find("c1"), child1);

			Datum& child2 = root.Append("c2", DatumTypes::Float);
			Datum& child3 = root.Append("c3", DatumTypes::Matrix);
			Assert::AreEqual(root[0], child1);
			Assert::AreEqual(root[1], child2);
			Assert::AreEqual(root[2], child3);

			const Scope& rootconst = root;
			Assert::AreEqual(rootconst[0], child1);
			Assert::AreEqual(rootconst[1], child2);
			Assert::AreEqual(rootconst[2], child3);
		}

		TEST_METHOD(Adopt)
		{
			Scope root;
			Scope child;
			Scope child2;

			Assert::ExpectException<std::runtime_error>([&root, &child] {root.Adopt(child, ""); });

			Assert::IsNull(child.GetParent());
			Assert::IsNull(child2.GetParent());
			root.Adopt(child, "c1");
			Assert::AreEqual(child.GetParent(), &root);
			root.Adopt(child2, "c1");
			Assert::AreEqual(child2.GetParent(), &root);

			Assert::ExpectException<std::runtime_error>([&root, &child] {child.Adopt(root, "cyclic"); });
		}

		TEST_METHOD(Equality)
		{
			Scope root;
			Scope child;
			Scope child2;
			root.Adopt(child, "c1");
			root.Append("c2") = 3;
			root.Adopt(child2, "c3");
			root.Append("c4") = glm::mat4(29.f);

			Scope root_2;
			Scope child_2;
			Scope child2_2;
			root_2.Adopt(child_2, "c1");
			root_2.Append("c2") = 3;
			root_2.Adopt(child2_2, "c3");
			root_2.Append("c4") = glm::mat4(29.f);

			Assert::AreEqual(root, root_2);
			Assert::AreNotEqual(root, child);
			Assert::AreNotEqual(root, child2);
			Assert::AreNotEqual(root, child_2);
			Assert::AreNotEqual(root, child2_2);
			Assert::AreNotEqual(root_2, child);
			Assert::AreNotEqual(root_2, child2);
			Assert::AreNotEqual(root_2, child_2);
			Assert::AreNotEqual(root_2, child2_2);
			Assert::IsTrue(root != child);

			RTTI* scopertti{ nullptr };
			Scope* nullscope{ nullptr };
			Assert::IsFalse(root.Equals(scopertti));
			Assert::IsFalse(root.Equals(nullscope));
			scopertti = &root_2;
			Assert::IsTrue(root.Equals(scopertti));
		}

		TEST_METHOD(Find)
		{
			Scope root;
			Scope child;
			Scope child2;

			root["c1"] = 3;
			root["c2"] = glm::mat4(2.f);
			root.Adopt(child, "c3");
			root.Adopt(child2, "c4");

			Assert::ExpectException<std::runtime_error>([&root] {root.Find(""); });

			Assert::IsNotNull(root.Find("c1"));
			Assert::IsNotNull(root.Find("c2"));
			Assert::IsNotNull(root.Find("c3"));
			Assert::IsNotNull(root.Find("c4"));
			Assert::IsNull(root.Find("c5"));

			child["cc1"] = 10;
			child["cc2"] = 10;
			child2["c2c1"] = 10;
			child2["c2c2"] = 10;
			Assert::IsNotNull(root.Find("cc1", true));
			Assert::IsNotNull(root.Find("cc2", true));
			Scope* foundScope{ nullptr };
			Assert::IsNotNull(root.Find("c2c1", true, &foundScope));
			Assert::IsNotNull(foundScope);
			foundScope = nullptr;
			Assert::IsNotNull(root.Find("c2c2", true, &foundScope));
			Assert::IsNotNull(foundScope);

			const Scope& rootconst = root;
			Assert::ExpectException<std::runtime_error>([&rootconst] {rootconst.Find(""); });
			Assert::IsNotNull(rootconst.Find("c1"));
			Assert::IsNotNull(rootconst.Find("c2"));
			Assert::IsNotNull(rootconst.Find("c3"));
			Assert::IsNotNull(rootconst.Find("c4"));
			Assert::IsNull(rootconst.Find("c5"));
			Assert::IsNotNull(rootconst.Find("cc1", true));
			Assert::IsNotNull(rootconst.Find("cc2", true));
			const Scope* foundScopeconst{ nullptr };
			Assert::IsNotNull(rootconst.Find("c2c1", true, &foundScopeconst));
			Assert::IsNotNull(foundScope);
			foundScopeconst = nullptr;
			Assert::IsNotNull(rootconst.Find("c2c2", true, &foundScopeconst));
			Assert::IsNotNull(foundScopeconst);
		}

		TEST_METHOD(Search)
		{
			Scope root;
			Scope child;
			Scope child2;

			root["c1"] = 3;
			root["c2"] = glm::mat4(2.f);
			root.Adopt(child, "c3");
			root.Adopt(child2, "c4");

			Assert::ExpectException<std::runtime_error>([&root] {root.Search(""); });

			Assert::IsNotNull(child.Search("c1"));
			Assert::AreEqual(child.Search("c1")->Type(), DatumTypes::Integer);
			Assert::AreEqual(child.Search("c1")->Get<std::int32_t>(), 3);
			Assert::IsNotNull(child.Search("c2"));
			Assert::AreEqual(child.Search("c2")->Type(), DatumTypes::Matrix);
			Assert::AreEqual(child.Search("c2")->Get<glm::mat4>(), glm::mat4(2.f));
			Assert::IsNotNull(child.Search("c3"));
			Assert::AreEqual(child.Search("c3")->Type(), DatumTypes::ScopeTable);
			Assert::AreEqual(child.Search("c3")->Get<Scope*>(), &child);
			Assert::IsNotNull(child.Search("c4"));
			Assert::AreEqual(child.Search("c4")->Type(), DatumTypes::ScopeTable);
			Assert::AreEqual(child.Search("c4")->Get<Scope*>(), &child2);
			Assert::IsNotNull(child2.Search("c1"));
			Assert::IsNotNull(child2.Search("c2"));
			Assert::IsNotNull(child2.Search("c3"));
			Assert::IsNotNull(child2.Search("c4"));

			Scope child_2;
			Scope child2_2;
			child["cc1"] = 10;
			child["cc2"] = 10;
			child.Adopt(child_2, "cc3");
			child2["c2c1"] = 10;
			child2["c2c2"] = 10;
			child2.Adopt(child2_2, "c2c3");
			Assert::IsNotNull(child_2.Search("c1"));
			Assert::IsNotNull(child_2.Search("c2"));
			Assert::IsNotNull(child_2.Search("c3"));
			Assert::IsNotNull(child_2.Search("c4"));
			Assert::IsNotNull(child_2.Search("cc1"));
			Assert::IsNotNull(child_2.Search("cc2"));
			Assert::IsNotNull(child_2.Search("cc3"));
			Scope* foundScope;
			Assert::IsNotNull(child2_2.Search("c1", foundScope));
			Assert::IsNotNull(foundScope);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2.Search("c2", foundScope));
			Assert::IsNotNull(foundScope);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2.Search("c3", foundScope));
			Assert::IsNotNull(foundScope);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2.Search("c4", foundScope));
			Assert::IsNotNull(foundScope);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2.Search("c2c1", foundScope));
			Assert::IsNotNull(foundScope);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2.Search("c2c2", foundScope));
			Assert::IsNotNull(foundScope);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2.Search("c2c3", foundScope));
			Assert::IsNotNull(foundScope);

			const Scope& rootconst = root;
			const Scope& childconst = child;
			const Scope& child2const = child2;

			Assert::ExpectException<std::runtime_error>([&rootconst] {rootconst.Search(""); });

			Assert::IsNotNull(childconst.Search("c1"));
			Assert::AreEqual(childconst.Search("c1")->Type(), DatumTypes::Integer);
			Assert::AreEqual(childconst.Search("c1")->Get<std::int32_t>(), 3);
			Assert::IsNotNull(childconst.Search("c2"));
			Assert::AreEqual(childconst.Search("c2")->Type(), DatumTypes::Matrix);
			Assert::AreEqual(childconst.Search("c2")->Get<glm::mat4>(), glm::mat4(2.f));
			Assert::IsNotNull(childconst.Search("c3"));
			Assert::AreEqual(childconst.Search("c3")->Type(), DatumTypes::ScopeTable);
			Assert::AreEqual(childconst.Search("c3")->Get<Scope*>(), &child);
			Assert::IsNotNull(childconst.Search("c4"));
			Assert::AreEqual(childconst.Search("c4")->Type(), DatumTypes::ScopeTable);
			Assert::AreEqual(childconst.Search("c4")->Get<Scope*>(), &child2);
			Assert::IsNotNull(child2const.Search("c1"));
			Assert::IsNotNull(child2const.Search("c2"));
			Assert::IsNotNull(child2const.Search("c3"));
			Assert::IsNotNull(child2const.Search("c4"));

			const Scope& child_2const = child_2;
			const Scope& child2_2const = child2_2;

			Assert::IsNotNull(child_2const.Search("c1"));
			Assert::IsNotNull(child_2const.Search("c2"));
			Assert::IsNotNull(child_2const.Search("c3"));
			Assert::IsNotNull(child_2const.Search("c4"));
			Assert::IsNotNull(child_2const.Search("cc1"));
			Assert::IsNotNull(child_2const.Search("cc2"));
			Assert::IsNotNull(child_2const.Search("cc3"));
			const Scope* foundScopeconst;
			Assert::IsNotNull(child2_2const.Search("c1", foundScopeconst));
			Assert::IsNotNull(foundScopeconst);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2const.Search("c2", foundScopeconst));
			Assert::IsNotNull(foundScopeconst);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2const.Search("c3", foundScopeconst));
			Assert::IsNotNull(foundScopeconst);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2const.Search("c4", foundScopeconst));
			Assert::IsNotNull(foundScopeconst);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2const.Search("c2c1", foundScopeconst));
			Assert::IsNotNull(foundScopeconst);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2const.Search("c2c2", foundScopeconst));
			Assert::IsNotNull(foundScopeconst);
			foundScope = nullptr;
			Assert::IsNotNull(child2_2const.Search("c2c3", foundScopeconst));
			Assert::IsNotNull(foundScopeconst);
		}

		TEST_METHOD(FindContainedScope)
		{
			Scope root;
			Scope child;
			Scope child2;

			root["c1"] = 3;
			root["c2"] = glm::mat4(2.f);
			root.Adopt(child, "c3");
			root.Adopt(child2, "c4");
			Assert::IsNotNull(root.FindContainedScope(child).first);
			Assert::AreEqual(root.FindContainedScope(child).first->Get<Scope*>(), &child);
			Assert::IsNotNull(root.FindContainedScope(child2).first);
			Assert::AreEqual(root.FindContainedScope(child2).first->Get<Scope*>(), &child2);

			Scope child_2;
			Scope child2_2;
			child["cc1"] = 10;
			child["cc2"] = 10;
			child.Adopt(child_2, "cc3");
			child2["c2c1"] = 10;
			child2["c2c2"] = 10;
			child2.Adopt(child2_2, "c2c3");
			Assert::IsNotNull(child.FindContainedScope(child_2).first);
			Assert::AreEqual(child.FindContainedScope(child_2).first->Get<Scope*>(), &child_2);
			Assert::IsNotNull(child2.FindContainedScope(child2_2).first);
			Assert::AreEqual(child2.FindContainedScope(child2_2).first->Get<Scope*>(), &child2_2);
		}

	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState ScopeTests::_startMemState;
}
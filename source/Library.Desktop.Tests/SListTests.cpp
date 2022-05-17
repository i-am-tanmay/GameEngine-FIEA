#include "pch.h"
#include "Foo.h"
#include "SList.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(SListTests)
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

#pragma region SLIST MANIPULATION
		TEST_METHOD(Constructor)
		{
			SList<Foo> list;
			Assert::AreEqual(list.Size(), std::size_t(0));
			Assert::IsTrue(list.IsEmpty());

			auto front = [&] {list.Front(); };
			Assert::ExpectException<std::runtime_error>(front);

			auto back = [&] {list.Back(); };
			Assert::ExpectException<std::runtime_error>(back);
		}

		TEST_METHOD(CopyConstructor)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			SList<Foo> listempty = list;
			Assert::IsTrue(listempty.IsEmpty());
			
			Foo f(20);
			list.PushBack(f);
			Foo fagain(30);
			list.PushBack(fagain);
			Foo fagainagain(40);
			list.PushBack(fagainagain);
			Assert::IsFalse(list.IsEmpty());

			SList<Foo> listagain = list;
			Assert::AreNotSame(list.Front(), listagain.Front());
			Assert::AreNotSame(list.Back(), listagain.Back());
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
			Assert::IsFalse(listagain.IsEmpty());
			Assert::AreEqual(f, listagain.Front());
			Assert::AreEqual(fagainagain, listagain.Back());
		}

		TEST_METHOD(CopyAssignment)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			SList<Foo> listagain;
			listagain = list;
			Assert::IsTrue(listagain.IsEmpty());

			Foo f(20);
			list.PushBack(f);
			Foo fagain(30);
			list.PushBack(fagain);
			Foo fagainagain(40);
			list.PushBack(fagainagain);
			Assert::IsFalse(list.IsEmpty());

			listagain = list;
			Assert::AreNotSame(list.Front(), listagain.Front());
			Assert::AreNotSame(list.Back(), listagain.Back());
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
			Assert::IsFalse(listagain.IsEmpty());
			Assert::AreEqual(f, listagain.Front());
			Assert::AreEqual(fagainagain, listagain.Back());

			list.PushBack(f);
			list.PushBack(fagain);

			listagain = list;
			Assert::AreNotSame(list.Front(), listagain.Front());
			Assert::AreNotSame(list.Back(), listagain.Back());
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
			Assert::IsFalse(listagain.IsEmpty());
			Assert::AreEqual(f, listagain.Front());
			Assert::AreEqual(fagain, listagain.Back());
		}

		TEST_METHOD(PushFront)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Foo f(20);
			list.PushFront(f);
			Assert::AreEqual(f, list.Front());
			Assert::AreEqual(f, list.Back());
			Assert::AreSame(list.Front(), list.Back());
			Assert::IsFalse(list.IsEmpty());

			Foo fagain(30);
			list.PushFront(fagain);
			Assert::AreEqual(fagain, list.Front());
			Assert::AreEqual(f, list.Back());
			Assert::IsFalse(list.IsEmpty());

			Foo fagainagain(40);
			list.PushFront(fagainagain);
			Assert::AreEqual(fagainagain, list.Front());
			Assert::AreEqual(f, list.Back());
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(PushBack)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Foo f(20);
			list.PushBack(f);
			Assert::AreEqual(f, list.Back());
			Assert::AreEqual(f, list.Front());
			Assert::AreSame(list.Front(), list.Back());
			Assert::IsFalse(list.IsEmpty());

			Foo fagain(30);
			list.PushBack(fagain);
			Assert::AreEqual(fagain, list.Back());
			Assert::AreEqual(f, list.Front());
			Assert::IsFalse(list.IsEmpty());

			Foo fagainagain(40);
			list.PushBack(fagainagain);
			Assert::AreEqual(fagainagain, list.Back());
			Assert::AreEqual(f, list.Front());
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(PopFront)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Foo f(20);
			list.PushBack(f);
			Foo fagain(30);
			list.PushBack(fagain);
			Foo fagainagain(40);
			list.PushBack(fagainagain);

			Assert::AreEqual(f, list.Front());
			Assert::AreEqual(fagainagain, list.Back());
			Assert::IsFalse(list.IsEmpty());

			list.PopFront();
			Assert::AreEqual(fagain, list.Front());
			Assert::AreEqual(fagainagain, list.Back());
			Assert::IsFalse(list.IsEmpty());

			list.PopFront();
			Assert::AreEqual(fagainagain, list.Front());
			Assert::AreEqual(fagainagain, list.Back());
			Assert::AreSame(list.Front(), list.Back());
			Assert::IsFalse(list.IsEmpty());

			list.PopFront();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(PopBack)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Foo f(20);
			list.PushFront(f);
			Foo fagain(30);
			list.PushFront(fagain);
			Foo fagainagain(40);
			list.PushFront(fagainagain);

			Assert::AreEqual(f, list.Back());
			Assert::AreEqual(fagainagain, list.Front());
			Assert::IsFalse(list.IsEmpty());

			list.PopBack();
			Assert::AreEqual(fagain, list.Back());
			Assert::AreEqual(fagainagain, list.Front());
			Assert::IsFalse(list.IsEmpty());

			list.PopBack();
			Assert::AreEqual(fagainagain, list.Back());
			Assert::AreEqual(fagainagain, list.Front());
			Assert::AreSame(list.Front(), list.Back());
			Assert::IsFalse(list.IsEmpty());

			list.PopBack();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(IsEmpty)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Foo f(20);
			Foo fagain(30);
			Foo fagainagain(40);

			list.PushFront(f);
			Assert::IsFalse(list.IsEmpty());
			list.PushFront(fagain);
			Assert::IsFalse(list.IsEmpty());
			list.PushFront(fagainagain);
			Assert::IsFalse(list.IsEmpty());

			list.PopFront();
			Assert::IsFalse(list.IsEmpty());
			list.PopFront();
			Assert::IsFalse(list.IsEmpty());
			list.PopFront();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			SList<Foo> list;
			Assert::AreEqual(list.Size(), std::size_t(0));

			Foo f(20);
			Foo fagain(30);
			Foo fagainagain(40);

			list.PushFront(f);
			Assert::AreEqual(list.Size(), std::size_t(1));
			list.PushFront(fagain);
			Assert::AreEqual(list.Size(), std::size_t(2));
			list.PushFront(fagainagain);
			Assert::AreEqual(list.Size(), std::size_t(3));

			list.PopFront();
			Assert::AreEqual(list.Size(), std::size_t(2));
			list.PopFront();
			Assert::AreEqual(list.Size(), std::size_t(1));
			list.PopFront();
			Assert::AreEqual(list.Size(), std::size_t(0));
		}

		TEST_METHOD(Clear)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Foo f(20);
			list.PushFront(f);
			Foo fagain(30);
			list.PushFront(fagain);
			Foo fagainagain(40);
			list.PushFront(fagainagain);

			Assert::IsFalse(list.IsEmpty());
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Front)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			auto front = [&] {list.Front(); };
			Assert::ExpectException<std::runtime_error>(front);

			Foo f(20);
			list.PushFront(f);
			Assert::AreEqual(f, list.Front());

			const SList<Foo>& listconst = list;
			Assert::AreEqual(f, listconst.Front());

			Foo fagain(30);
			list.PushFront(fagain);
			Assert::AreEqual(fagain, list.Front());

			const SList<Foo>& listconstagain = list;
			Assert::AreEqual(fagain, listconstagain.Front());

			Foo fagainagain(40);
			list.PushFront(fagainagain);
			Assert::AreEqual(fagainagain, list.Front());

			const SList<Foo>& listconstagainagain = list;
			Assert::AreEqual(fagainagain, listconstagainagain.Front());

			list.Clear();
			Assert::ExpectException<std::runtime_error>(front);

			const SList<Foo>& listconstempty = list;
			auto frontconst = [&] {listconstempty.Front(); };
			Assert::ExpectException<std::runtime_error>(frontconst);
		}

		TEST_METHOD(Back)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			auto back = [&] {list.Back(); };
			Assert::ExpectException<std::runtime_error>(back);

			Foo f(20);
			list.PushBack(f);
			Assert::AreEqual(f, list.Back());

			const SList<Foo>& listconst = list;
			Assert::AreEqual(f, listconst.Back());

			Foo fagain(30);
			list.PushBack(fagain);
			Assert::AreEqual(fagain, list.Back());

			const SList<Foo>& listconstagain = list;
			Assert::AreEqual(fagain, listconstagain.Back());

			Foo fagainagain(40);
			list.PushBack(fagainagain);
			Assert::AreEqual(fagainagain, list.Back());

			const SList<Foo>& listconstagainagain = list;
			Assert::AreEqual(fagainagain, listconstagainagain.Back());

			list.Clear();
			Assert::ExpectException<std::runtime_error>(back);

			const SList<Foo>& listconstempty = list;
			auto frontconst = [&] {listconstempty.Back(); };
			Assert::ExpectException<std::runtime_error>(frontconst);
		}
#pragma endregion

#pragma region SLIST ITERATORS
		TEST_METHOD(ITRConstructorDefault)
		{
			Assert::ExpectException<std::runtime_error>([] {SList<Foo>::Iterator itr; ++itr; });
			Assert::ExpectException<std::runtime_error>([] {SList<Foo>::Const_Iterator itr; ++itr; });
		}

		TEST_METHOD(ITRConstructorCopy)
		{
			SList<Foo>::Iterator itr;
			SList<Foo>::Iterator itragain = itr;
			Assert::AreEqual(itr, itragain);

			SList<Foo> list;
			SList<Foo>::Iterator itragainagain = list.begin();
			Assert::AreNotEqual(itragainagain, itragain);

			SList<Foo>::Const_Iterator const_itr;
			SList<Foo>::Const_Iterator const_itragain = const_itr;
			Assert::AreEqual(const_itr, const_itragain);

			SList<Foo> const_list;
			SList<Foo>::Const_Iterator const_itragainagain = const_list.begin();
			Assert::AreNotEqual(const_itragainagain, const_itragain);
		}

		TEST_METHOD(ITRAssignment)
		{
			SList<Foo>::Iterator itr;
			SList<Foo>::Iterator itragain;
			SList<Foo> list;
			itr = list.begin();
			Assert::AreNotEqual(itr, itragain);
			itragain = itr;
			Assert::AreEqual(itr, itragain);

			SList<Foo>::Const_Iterator const_itr;
			SList<Foo>::Const_Iterator const_itragain;
			SList<Foo> const_list;
			const_itr = const_list.begin();
			Assert::AreNotEqual(const_itr, const_itragain);
			const_itragain = const_itr;
			Assert::AreEqual(const_itr, const_itragain);
		}

		TEST_METHOD(ITRPreIncrement)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			SList<Foo> list;
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			for (SList<Foo>::Iterator itr = list.begin(); itr != list.end();)
			{
				Assert::AreEqual(list.Front(), *itr);
				++itr;
				list.PopFront();
			}

			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			for (SList<Foo>::Const_Iterator itr = list.begin(); itr != list.end();)
			{
				Assert::AreEqual(list.Front(), *itr);
				++itr;
				list.PopFront();
			}
		}

		TEST_METHOD(ITRPostIncrement)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			SList<Foo> list;
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			for (SList<Foo>::Iterator itr = list.begin(); itr != list.end();)
			{
				Assert::AreEqual(list.Front(), *itr);
				SList<Foo>::Iterator itrbefore = itr++;
				Assert::AreEqual(list.Front(), *itrbefore);
				if (itr != list.end()) Assert::AreNotEqual(list.Front(), *itr);
				list.PopFront();
			}

			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			for (SList<Foo>::Const_Iterator itr = list.begin(); itr != list.end();)
			{
				Assert::AreEqual(list.Front(), *itr);
				SList<Foo>::Const_Iterator itrbefore = itr++;
				Assert::AreEqual(list.Front(), *itrbefore);
				if (itr != list.end()) Assert::AreNotEqual(list.Front(), *itr);
				list.PopFront();
			}
		}

		TEST_METHOD(begin)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			SList<Foo> list;

			list.PushFront(fagainagainagain);
			Assert::AreEqual(*(list.begin()), fagainagainagain);
			list.PushFront(fagainagain);
			Assert::AreEqual(*(list.begin()), fagainagain);
			list.PushFront(fagain);
			Assert::AreEqual(*(list.begin()), fagain);
			list.PushFront(f);
			Assert::AreEqual(*(list.begin()), f);

			list.PopFront();
			Assert::AreEqual(*(list.begin()), fagain);
			list.PopFront();
			Assert::AreEqual(*(list.begin()), fagainagain);
			list.PopFront();
			Assert::AreEqual(*(list.begin()), fagainagainagain);
			list.PopFront();
			Assert::AreEqual(list.begin(), list.end());

			

			list.PushFront(fagainagainagain);
			const SList<Foo> const_list = list;
			Assert::AreEqual(*(const_list.begin()), fagainagainagain);
			list.PushFront(fagainagain);
			const SList<Foo> const_listagain = list;
			Assert::AreEqual(*(const_listagain.begin()), fagainagain);
			list.PushFront(fagain);
			const SList<Foo> const_listagainagain = list;
			Assert::AreEqual(*(const_listagainagain.begin()), fagain);
			list.PushFront(f);
			const SList<Foo> const_listagainagainagain = list;
			Assert::AreEqual(*(const_listagainagainagain.begin()), f);

			list.PopFront();
			Assert::AreEqual(*(list.cbegin()), fagain);
			list.PopFront();
			Assert::AreEqual(*(list.cbegin()), fagainagain);
			list.PopFront();
			Assert::AreEqual(*(list.cbegin()), fagainagainagain);
			list.PopFront();
			Assert::AreEqual(list.cbegin(), list.cend());
		}

		TEST_METHOD(end)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			SList<Foo> list;
			Assert::AreEqual(list.begin(), list.end());
			Assert::AreEqual(list.cbegin(), list.cend());

			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			Assert::AreNotEqual(list.begin(), list.end());
			Assert::AreNotEqual(list.cbegin(), list.cend());

			list.PopFront();
			list.PopFront();
			list.PopFront();
			list.PopFront();
			Assert::AreEqual(list.begin(), list.end());
			Assert::AreEqual(list.cbegin(), list.cend());

			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			const SList<Foo> const_list = list;
			Assert::AreNotEqual(const_list.begin(), const_list.end());

			list.PopFront();
			list.PopFront();
			list.PopFront();
			list.PopFront();

			const SList<Foo> const_listagain = list;
			Assert::AreEqual(const_listagain.begin(), const_listagain.end());
		}

		TEST_METHOD(InsertAfter)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			SList<Foo> list;
			SList<Foo>::Iterator itrnull;
			Assert::ExpectException<std::runtime_error>([&list, &itrnull, &f] {list.InsertAfter(itrnull, f); });
			SList<Foo>::Iterator itr = list.begin();
			list.InsertAfter(itr, f);
			itr = list.begin();
			Assert::AreEqual(*itr, f);
			list.InsertAfter(itr, fagain);
			Assert::AreEqual(*(itr++), f);
			Assert::AreEqual(*itr, fagain);
			itr = list.begin();
			list.InsertAfter(itr, fagainagain);
			Assert::AreEqual(*(itr++), f);
			Assert::AreEqual(*(itr++), fagainagain);
			Assert::AreEqual(*itr, fagain);
		}

		TEST_METHOD(Find)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			SList<Foo> list;
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			Assert::AreEqual(*(list.Find(f)), f);
			Assert::AreEqual(*(list.Find(fagain)), fagain);
			Assert::AreEqual(*(list.Find(fagainagain)), fagainagain);
			Assert::AreEqual(*(list.Find(fagainagainagain)), fagainagainagain);

			Foo fnotinlist{ 50 };
			Assert::AreEqual(list.Find(fnotinlist), list.end());

			const SList<Foo> const_list = list;
			Assert::AreEqual(*(const_list.Find(f)), f);
			Assert::AreEqual(*(const_list.Find(fagain)), fagain);
			Assert::AreEqual(*(const_list.Find(fagainagain)), fagainagain);
			Assert::AreEqual(*(const_list.Find(fagainagainagain)), fagainagainagain);

			Assert::AreEqual(const_list.Find(fnotinlist), const_list.end());
		}

		TEST_METHOD(Remove)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			SList<Foo> list;
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);
			list.PushBack(f);
			list.PushBack(fagain);

			Assert::AreEqual(*(list.Find(f)), f);
			SList<Foo>::Iterator itrnull;
			Assert::ExpectException<std::runtime_error>([&list, &itrnull, &f] {list.Remove(f, list.begin(), itrnull); });
			Assert::ExpectException<std::runtime_error>([&list, &itrnull, &f] {list.Remove(f, itrnull, list.end()); });
			list.Remove(f);
			Assert::AreNotEqual(list.Find(f), list.end());
			list.Remove(f);
			Assert::AreEqual(list.Find(f), list.end());

			Assert::AreEqual(*(list.Find(fagain)), fagain);
			list.Remove(fagain);
			Assert::AreNotEqual(list.Find(fagain), list.end());
			list.Remove(fagain);
			Assert::AreEqual(list.Find(fagain), list.end());

			Assert::AreEqual(*(list.Find(fagainagain)), fagainagain);
			list.Remove(fagainagain);
			Assert::AreEqual(list.Find(fagainagain), list.end());

			Assert::AreEqual(*(list.Find(fagainagainagain)), fagainagainagain);
			list.Remove(fagainagainagain);
			Assert::AreEqual(list.Find(fagainagainagain), list.end());

			list.Clear();
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);
			
			Assert::AreEqual(*(list.Find(fagain)), fagain);
			list.Remove(list.Find(fagain));
			Assert::AreEqual(list.Find(fagain), list.end());
			
			Assert::AreEqual(*(list.Find(f)), f);
			list.Remove(list.Find(f));
			Assert::AreEqual(list.Find(f), list.end());

			Assert::AreEqual(*(list.Find(fagainagainagain)), fagainagainagain);
			list.Remove(list.Find(fagainagainagain));
			Assert::AreEqual(list.Find(fagainagainagain), list.end());

			Assert::AreEqual(*(list.Find(fagainagain)), fagainagain);
			list.Remove(list.Find(fagainagain));
			Assert::AreEqual(list.Find(fagainagain), list.end());
		}

		TEST_METHOD(RemoveAll)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			SList<Foo> list;
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);
			list.PushBack(fagainagainagain);
			list.PushBack(fagainagain);
			list.PushBack(fagain);
			list.PushBack(f);
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			Assert::AreEqual(*(list.Find(f)), f);
			SList<Foo>::Iterator itrnull;
			Assert::ExpectException<std::runtime_error>([&list, &itrnull, &f] {list.RemoveAll(f, list.begin(), itrnull); });
			Assert::ExpectException<std::runtime_error>([&list, &itrnull, &f] {list.RemoveAll(f, itrnull, list.end()); });
			list.RemoveAll(f);
			Assert::AreEqual(list.Find(f), list.end());
		}
#pragma endregion


	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState SListTests::_startMemState;
}
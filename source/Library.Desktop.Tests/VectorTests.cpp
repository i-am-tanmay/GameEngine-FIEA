#include "pch.h"
#include "Foo.h"
#include "Vector.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(VectorTests)
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
			Vector<Foo> list;
			Assert::AreEqual(list.Size(), std::size_t(0));
			Assert::IsTrue(list.IsEmpty());

			auto front = [&] {list.Front(); };
			Assert::ExpectException<std::runtime_error>(front);

			auto back = [&] {list.Back(); };
			Assert::ExpectException<std::runtime_error>(back);
		}

		TEST_METHOD(ConstructorInitList)
		{
			Foo f{ 20 };
			Foo fagain{ 30 };
			Foo fagainagain{ 40 };
			Vector<Foo> list{ f,fagain,fagainagain };
			Assert::AreEqual(list.Size(), std::size_t(3));
			Assert::AreEqual(list.Capacity(), std::size_t(3));

			Vector<Foo>::Iterator itr = list.begin();
			Assert::AreEqual(*(itr++), f);
			Assert::AreEqual(*(itr++), fagain);
			Assert::AreEqual(*(itr++), fagainagain);
			Assert::AreEqual(itr, list.end());
		}

		TEST_METHOD(CopyConstructor)
		{
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Vector<Foo> listempty = list;
			Assert::IsTrue(listempty.IsEmpty());

			Foo f(20);
			list.PushBack(f);
			Foo fagain(30);
			list.PushBack(fagain);
			Foo fagainagain(40);
			list.PushBack(fagainagain);
			Assert::IsFalse(list.IsEmpty());

			Vector<Foo> listagain = list;
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
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Vector<Foo> listagain;
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

		TEST_METHOD(MoveConstructor)
		{
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Vector<Foo> listempty = std::move(list);
			Assert::IsTrue(listempty.IsEmpty());

			Foo f(20);
			list.PushBack(f);
			Foo fagain(30);
			list.PushBack(fagain);
			Foo fagainagain(40);
			list.PushBack(fagainagain);
			Assert::IsFalse(list.IsEmpty());

			Vector<Foo> listagain = std::move(list);
			Assert::IsTrue(list.IsEmpty());
			Assert::IsFalse(listagain.IsEmpty());
			Assert::AreEqual(f, listagain.Front());
			Assert::AreEqual(fagainagain, listagain.Back());
		}

		TEST_METHOD(MoveAssignment)
		{
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Vector<Foo> listagain;
			listagain = std::move(list);
			Assert::IsTrue(listagain.IsEmpty());

			Foo f(20);
			list.PushBack(f);
			Foo fagain(30);
			list.PushBack(fagain);
			Foo fagainagain(40);
			list.PushBack(fagainagain);
			Assert::IsFalse(list.IsEmpty());

			listagain = std::move(list);
			Assert::IsTrue(list.IsEmpty());
			Assert::IsFalse(listagain.IsEmpty());
			Assert::IsTrue(list.IsEmpty());
			Assert::IsFalse(listagain.IsEmpty());
			Assert::AreEqual(f, listagain.Front());
			Assert::AreEqual(fagainagain, listagain.Back());

			list.PushBack(f);
			list.PushBack(fagain);

			listagain = std::move(list);
			Assert::IsTrue(list.IsEmpty());
			Assert::IsFalse(listagain.IsEmpty());
			Assert::IsTrue(list.IsEmpty());
			Assert::IsFalse(listagain.IsEmpty());
			Assert::AreEqual(f, listagain.Front());
			Assert::AreEqual(fagain, listagain.Back());
		}

		TEST_METHOD(At)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			Vector<Foo> list;
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			Assert::AreEqual(list.At(0), f);
			Assert::AreEqual(list.At(1), fagain);
			Assert::AreEqual(list.At(2), fagainagain);
			Assert::AreEqual(list.At(3), fagainagainagain);

			Assert::AreEqual(list[0], f);
			Assert::AreEqual(list[1], fagain);
			Assert::AreEqual(list[2], fagainagain);
			Assert::AreEqual(list[3], fagainagainagain);

			const Vector<Foo> constlist = list;

			Assert::AreEqual(constlist.At(0), f);
			Assert::AreEqual(constlist.At(1), fagain);
			Assert::AreEqual(constlist.At(2), fagainagain);
			Assert::AreEqual(constlist.At(3), fagainagainagain);

			Assert::AreEqual(constlist[0], f);
			Assert::AreEqual(constlist[1], fagain);
			Assert::AreEqual(constlist[2], fagainagain);
			Assert::AreEqual(constlist[3], fagainagainagain);
		}

		TEST_METHOD(ShrinkToFit)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };

			Vector<Foo> list;
			Assert::AreEqual(list.Capacity(), list.Size());

			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			Assert::AreNotEqual(list.Capacity(), list.Size());

			list.ShrinkToFit();
			Assert::AreEqual(list.Capacity(), list.Size());

			list.Clear();
			Assert::AreNotEqual(list.Capacity(), list.Size());
			list.ShrinkToFit();
			Assert::AreEqual(list.Capacity(), list.Size());
			Assert::AreEqual(list.Capacity(), std::size_t(0));
			Assert::AreEqual(list.Size(), std::size_t(0));
		}

		static std::size_t my_increment(std::size_t size, std::size_t capacity)
		{
			size;
			if (capacity > 0u) return capacity * 2u;
			else return 10u;
		}

		TEST_METHOD(PushBack)
		{
			Vector<Foo> list;
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

			

			list.SetIncrementFunctor(my_increment);
			Foo fagainagainagain(50);
			list.PushBack(std::move(fagainagainagain));
			Assert::AreEqual(fagainagainagain, list.Back());
			fagainagainagain.Data() = 60;
			Assert::AreNotEqual(fagainagainagain, list.Back());
		}

		TEST_METHOD(PopBack)
		{
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Foo f(20);
			Foo fagain(30);
			Foo fagainagain(40);
			list.PushBack(fagainagain);
			list.PushBack(fagain);
			list.PushBack(f);

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
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Foo f(20);
			Foo fagain(30);
			Foo fagainagain(40);

			list.PushBack(f);
			Assert::IsFalse(list.IsEmpty());
			list.PushBack(fagain);
			Assert::IsFalse(list.IsEmpty());
			list.PushBack(fagainagain);
			Assert::IsFalse(list.IsEmpty());

			list.PopBack();
			Assert::IsFalse(list.IsEmpty());
			list.PopBack();
			Assert::IsFalse(list.IsEmpty());
			list.PopBack();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(SizeCapacity)
		{
			Vector<Foo> list(10u);
			Assert::AreEqual(list.Capacity(), std::size_t(10u));
			Assert::AreEqual(list.Size(), std::size_t(0));

			Foo f(20);
			Foo fagain(30);
			Foo fagainagain(40);

			list.PushBack(f);
			Assert::AreEqual(list.Size(), std::size_t(1));
			list.PushBack(fagain);
			Assert::AreEqual(list.Size(), std::size_t(2));
			list.PushBack(fagainagain);
			Assert::AreEqual(list.Size(), std::size_t(3));

			list.PopBack();
			Assert::AreEqual(list.Size(), std::size_t(2));
			list.PopBack();
			Assert::AreEqual(list.Size(), std::size_t(1));
			list.PopBack();
			Assert::AreEqual(list.Size(), std::size_t(0));
		}

		TEST_METHOD(Clear)
		{
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			Foo f(20);
			list.PushBack(f);
			Foo fagain(30);
			list.PushBack(fagain);
			Foo fagainagain(40);
			list.PushBack(fagainagain);

			Assert::IsFalse(list.IsEmpty());
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Front)
		{
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			auto front = [&] {list.Front(); };
			Assert::ExpectException<std::runtime_error>(front);

			Foo f(20);
			list.PushBack(f);
			Assert::AreEqual(f, list.Front());

			const Vector<Foo>& listconst = list;
			Assert::AreEqual(f, listconst.Front());

			Foo fagain(30);
			list.PushBack(fagain);
			Assert::AreEqual(f, list.Front());

			const Vector<Foo>& listconstagain = list;
			Assert::AreEqual(f, listconstagain.Front());

			Foo fagainagain(40);
			list.PushBack(fagainagain);
			Assert::AreEqual(f, list.Front());

			const Vector<Foo>& listconstagainagain = list;
			Assert::AreEqual(f, listconstagainagain.Front());

			list.Clear();
			Assert::ExpectException<std::runtime_error>(front);

			const Vector<Foo>& listconstempty = list;
			auto frontconst = [&] {listconstempty.Front(); };
			Assert::ExpectException<std::runtime_error>(frontconst);
		}

		TEST_METHOD(Back)
		{
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			auto back = [&] {list.Back(); };
			Assert::ExpectException<std::runtime_error>(back);

			Foo f(20);
			list.PushBack(f);
			Assert::AreEqual(f, list.Back());

			const Vector<Foo>& listconst = list;
			Assert::AreEqual(f, listconst.Back());

			Foo fagain(30);
			list.PushBack(fagain);
			Assert::AreEqual(fagain, list.Back());

			const Vector<Foo>& listconstagain = list;
			Assert::AreEqual(fagain, listconstagain.Back());

			Foo fagainagain(40);
			list.PushBack(fagainagain);
			Assert::AreEqual(fagainagain, list.Back());

			const Vector<Foo>& listconstagainagain = list;
			Assert::AreEqual(fagainagain, listconstagainagain.Back());

			list.Clear();
			Assert::ExpectException<std::runtime_error>(back);

			const Vector<Foo>& listconstempty = list;
			auto frontconst = [&] {listconstempty.Back(); };
			Assert::ExpectException<std::runtime_error>(frontconst);
		}
#pragma endregion

#pragma region SLIST ITERATORS
		TEST_METHOD(ITRConstructorDefault)
		{
			Assert::ExpectException<std::runtime_error>([] {Vector<Foo>::Iterator itr; ++itr; });
			Assert::ExpectException<std::runtime_error>([] {Vector<Foo>::Const_Iterator itr; ++itr; });
		}

		TEST_METHOD(ITRConstructorCopy)
		{
			Vector<Foo>::Iterator itr;
			Vector<Foo>::Iterator itragain = itr;
			Assert::AreEqual(itr, itragain);

			Vector<Foo> list;
			Vector<Foo>::Iterator itragainagain = list.begin();
			Assert::AreNotEqual(itragainagain, itragain);

			Vector<Foo>::Const_Iterator const_itr;
			Vector<Foo>::Const_Iterator const_itragain = const_itr;
			Assert::AreEqual(const_itr, const_itragain);

			Vector<Foo> const_list;
			Vector<Foo>::Const_Iterator const_itragainagain = const_list.begin();
			Assert::AreNotEqual(const_itragainagain, const_itragain);
		}

		TEST_METHOD(ITRAssignment)
		{
			Vector<Foo>::Iterator itr;
			Vector<Foo>::Iterator itragain;
			Vector<Foo> list;
			itr = list.begin();
			Assert::AreNotEqual(itr, itragain);
			itragain = itr;
			Assert::AreEqual(itr, itragain);

			Vector<Foo>::Const_Iterator const_itr;
			Vector<Foo>::Const_Iterator const_itragain;
			Vector<Foo> const_list;
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

			Vector<Foo> list;
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			{
				Vector<Foo>::Iterator itr = list.begin();
				Assert::AreEqual(f, *itr);
				++itr;
				Assert::AreEqual(fagain, *itr);
				++itr;
				Assert::AreEqual(fagainagain, *itr);
				++itr;
				Assert::AreEqual(fagainagainagain, *itr);
			}

			{
				Vector<Foo>::Const_Iterator itr = list.begin();
				Assert::AreEqual(f, *itr);
				++itr;
				Assert::AreEqual(fagain, *itr);
				++itr;
				Assert::AreEqual(fagainagain, *itr);
				++itr;
				Assert::AreEqual(fagainagainagain, *itr);
			}
		}

		TEST_METHOD(ITRPostIncrement)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			Vector<Foo> list;
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			{
				Vector<Foo>::Iterator itr = list.begin();
				Assert::AreEqual(f, *(itr++));
				Assert::AreEqual(fagain, *(itr++));
				Assert::AreEqual(fagainagain, *(itr++));
				Assert::AreEqual(fagainagainagain, *(itr++));
			}

			{
				Vector<Foo>::Const_Iterator itr = list.begin();
				Assert::AreEqual(f, *(itr++));
				Assert::AreEqual(fagain, *(itr++));
				Assert::AreEqual(fagainagain, *(itr++));
				Assert::AreEqual(fagainagainagain, *(itr++));
			}
		}

		TEST_METHOD(begin)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			Vector<Foo> list;

			list.PushBack(f);
			Assert::AreEqual(*(list.begin()), f);
			list.PushBack(fagain);
			Assert::AreEqual(*(list.begin()), f);

			list.PopBack();
			Assert::AreEqual(*(list.begin()), f);
			list.PopBack();
			Assert::AreEqual(list.begin(), list.end());



			list.PushBack(f);
			const Vector<Foo> const_list = list;
			Assert::AreEqual(*(const_list.begin()), f);
			list.PushBack(fagain);
			const Vector<Foo> const_listagain = list;
			Assert::AreEqual(*(const_listagain.begin()), f);

			list.PopBack();
			Assert::AreEqual(*(list.cbegin()), f);
			list.PopBack();
			Assert::AreEqual(list.cbegin(), list.cend());
		}

		TEST_METHOD(end)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			Vector<Foo> list;
			Assert::AreEqual(list.begin(), list.end());
			Assert::AreEqual(list.cbegin(), list.cend());

			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			Assert::AreNotEqual(list.begin(), list.end());
			Assert::AreNotEqual(list.cbegin(), list.cend());

			list.PopBack();
			list.PopBack();
			list.PopBack();
			list.PopBack();
			Assert::AreEqual(list.begin(), list.end());
			Assert::AreEqual(list.cbegin(), list.cend());

			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);

			const Vector<Foo> const_list = list;
			Assert::AreNotEqual(const_list.begin(), const_list.end());

			list.PopBack();
			list.PopBack();
			list.PopBack();
			list.PopBack();

			const Vector<Foo> const_listagain = list;
			Assert::AreEqual(const_listagain.begin(), const_listagain.end());
		}

		TEST_METHOD(Find)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			Vector<Foo> list;
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

			const Vector<Foo> const_list = list;
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

			Vector<Foo> list;
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);
			list.PushBack(f);
			list.PushBack(fagain);

			Assert::AreEqual(*(list.Find(f)), f);
			Vector<Foo>::Iterator itrnull;
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

			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);
			Assert::AreEqual(*list.begin(), f);
			list.Remove(list.begin());
			Assert::AreEqual(*list.begin(), fagain);
			list.Remove(list.begin());
			Assert::AreEqual(*list.begin(), fagainagain);
			list.Remove(list.begin());
			Assert::AreEqual(*list.begin(), fagainagainagain);
			list.Remove(list.begin());
			Assert::AreEqual(list.begin(), list.end());
		}

		TEST_METHOD(RemoveAll)
		{
			Foo f{ 10 };
			Foo fagain{ 20 };
			Foo fagainagain{ 30 };
			Foo fagainagainagain{ 40 };

			Vector<Foo> list;
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
			Vector<Foo>::Iterator itrnull;
			Assert::ExpectException<std::runtime_error>([&list, &itrnull, &f] {list.RemoveAll(f, list.begin(), itrnull); });
			Assert::ExpectException<std::runtime_error>([&list, &itrnull, &f] {list.RemoveAll(f, itrnull, list.end()); });
			list.RemoveAll(f);
			Assert::AreEqual(list.Find(f), list.end());
			
			list.Clear();
			list.PushBack(f);
			list.PushBack(fagain);
			list.PushBack(fagainagain);
			list.PushBack(fagainagainagain);
			Vector<Foo>::Iterator itr = list.begin(); ++itr; ++itr;
			list.RemoveAll(list.begin(), itr);
			Assert::AreEqual(*list.begin(), fagainagain);
		}
#pragma endregion

	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState VectorTests::_startMemState;
}
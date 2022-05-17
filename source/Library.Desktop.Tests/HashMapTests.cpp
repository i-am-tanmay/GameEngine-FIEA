#include "pch.h"
#include "Foo.h"
#include "HashMap.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(HashMapTests)
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

		struct FooHashFunct final
		{
			std::size_t operator()(const Foo& key) const
			{
				return abs(key.Data());
			}
		};

		struct FooHashFunctPrime final
		{
			std::size_t operator()(const Foo& key) const
			{
				return abs(key.Data() * 7);
			}
		};

		struct FooHashFunctZero final
		{
			std::size_t operator()(const Foo&) const
			{
				return 0;
			}
		};

		struct FooEqualityFunct final
		{
			bool operator()(const Foo& lhs, const Foo& rhs) const
			{
				return lhs.Data() == rhs.Data();
			}
		};

		struct FooEqualityFunctError final
		{
			bool operator()(const Foo&, const Foo&) const
			{
				return true;
			}
		};

#pragma region HASHMAP TESTS
		TEST_METHOD(Constructor)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };
			Assert::AreEqual(hash.begin(), hash.end());
			Assert::AreEqual(hash.Size(), std::size_t(0u));
			Assert::IsTrue(hash.IsEmpty());
		}

		TEST_METHOD(MOVESymantics)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);
			hash.Insert(f2, f1);
			Assert::AreEqual(hash.Size(), std::size_t(2u));

			HashMap<Foo, Foo> hashmove = std::move(hash);
			Assert::AreEqual(hash.Size(), std::size_t(0u));
			Assert::AreEqual(hashmove.Size(), std::size_t(2u));
			Assert::IsTrue(hashmove.ContainsKey(f1));
			Assert::IsTrue(hashmove.ContainsKey(f2));

			HashMap<Foo, Foo> hashmoveequal;
			hashmoveequal = std::move(hashmove);
			Assert::AreEqual(hashmove.Size(), std::size_t(0u));
			Assert::AreEqual(hashmoveequal.Size(), std::size_t(2u));
			Assert::IsTrue(hashmoveequal.ContainsKey(f1));
			Assert::IsTrue(hashmoveequal.ContainsKey(f2));
		}

		TEST_METHOD(begin)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };
			Assert::AreEqual(hash.begin(), hash.end());
			Assert::AreEqual(hash.cbegin(), hash.cend());

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);
			Assert::AreEqual(*(hash.begin()), std::pair<const Foo, Foo>(f1, f2));
			Assert::AreEqual(*(hash.cbegin()), std::pair<const Foo, Foo>(f1, f2));
			Assert::AreNotEqual(hash.begin(), hash.end());
			Assert::AreNotEqual(hash.cbegin(), hash.cend());

			hash.Clear();
			hash.Insert(f2, f1);
			Assert::AreEqual(*(hash.begin()), std::pair<const Foo, Foo>(f2, f1));
			Assert::AreEqual(*(hash.cbegin()), std::pair<const Foo, Foo>(f2, f1));
			Assert::AreNotEqual(hash.begin(), hash.end());
			Assert::AreNotEqual(hash.cbegin(), hash.cend());
		}

		TEST_METHOD(end)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };
			Assert::AreEqual(hash.begin(), hash.end());

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);
			Assert::AreNotEqual(hash.begin(), hash.end());
			Assert::AreEqual(++hash.begin(), hash.end());

			hash.Clear();
			Assert::AreEqual(hash.begin(), hash.end());
			Assert::AreEqual(++hash.begin(), hash.end());
		}

		TEST_METHOD(SizeEmptyLoadFactor)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };
			Assert::IsTrue(hash.IsEmpty());
			Assert::AreEqual(hash.Size(), std::size_t(0u));
			Assert::AreEqual(hash.LoadFactor(), 0.f);

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);
			Assert::IsFalse(hash.IsEmpty());
			Assert::AreEqual(hash.Size(), std::size_t(1u));
			Assert::AreNotEqual(hash.LoadFactor(), 0.f);

			hash.Insert(f2, f1);
			Assert::IsFalse(hash.IsEmpty());
			Assert::AreEqual(hash.Size(), std::size_t(2u));
			Assert::AreNotEqual(hash.LoadFactor(), 0.f);

			hash.Clear();
			Assert::IsTrue(hash.IsEmpty());
			Assert::AreEqual(hash.Size(), std::size_t(0u));
			Assert::AreEqual(hash.LoadFactor(), 0.f);
		}

		TEST_METHOD(Find)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };

			Foo f1{ 10 };
			Foo f2{ 20 };

			Assert::AreEqual(hash.Find(f1), hash.end());
			hash.Insert(f1, f2);
			Assert::AreNotEqual(hash.Find(f1), hash.end());

			Assert::AreEqual(hash.Find(f2), hash.end());
			hash.Insert(f2, f1);
			Assert::AreNotEqual(hash.Find(f1), hash.end());

			const HashMap<Foo, Foo> hashconst = hash;
			Assert::AreNotEqual(hashconst.Find(f1), hash.cend());
			Assert::AreNotEqual(hashconst.Find(f2), hash.cend());
		}

		TEST_METHOD(ContainsKey)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };

			Foo f1{ 10 };
			Foo f2{ 20 };

			Assert::IsFalse(hash.ContainsKey(f1));
			hash.Insert(f1, f2);
			Assert::IsTrue(hash.ContainsKey(f1));

			Assert::IsFalse(hash.ContainsKey(f2));
			hash.Insert(f2, f1);
			Assert::IsTrue(hash.ContainsKey(f2));
		}

		TEST_METHOD(Insert)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };

			Foo f1{ 10 };
			Foo f2{ 20 };

			Assert::IsFalse(hash.ContainsKey(f1));
			Assert::IsTrue(hash.Insert(f1, f2).second);
			Assert::IsTrue(hash.ContainsKey(f1));
			Assert::IsFalse(hash.Insert(f1, f1).second);

			Assert::IsFalse(hash.ContainsKey(f2));
			Assert::IsTrue(hash.Insert(f2, f1).second);
			Assert::IsTrue(hash.ContainsKey(f2));
			Assert::IsFalse(hash.Insert(f2, f2).second);
		}

		TEST_METHOD(AtOperator)
		{
			HashMap<Foo, int> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };

			Foo f1{ 10 };
			Foo f2{ 20 };

			Assert::ExpectException<std::out_of_range>([&hash, &f1] {hash.At(f1); });
			hash[f1] = 30;
			Assert::AreEqual(hash.At(f1), 30);

			Assert::ExpectException<std::out_of_range>([&hash, &f2] {hash.At(f2); });
			Assert::IsFalse(hash.ContainsKey(f2));
			hash[f2];
			Assert::IsTrue(hash.ContainsKey(f2));
			hash[f2] = 40;
			Assert::AreEqual(hash.At(f2), 40);

			int* hdata{ nullptr };
			Assert::IsTrue(hash.ContainsKey(f1, hdata));
			if (hdata) *hdata = 50;
			Assert::IsTrue(hash.ContainsKey(f2, hdata));
			if (hdata) *hdata = 60;

			const int* hdataconst{ nullptr };
			const HashMap<Foo, int> hashconst = hash;
			Assert::AreEqual(hashconst.At(f1), 50);
			Assert::AreEqual(hashconst.At(f2), 60);
			Assert::IsTrue(hashconst.ContainsKey(f1, hdataconst));
			Assert::AreEqual(*hdataconst, 50);
			Assert::IsTrue(hashconst.ContainsKey(f2, hdataconst));
			Assert::AreEqual(*hdataconst, 60);
		}

		TEST_METHOD(Remove)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunctZero{}, FooEqualityFunct{} };

			Foo f1{ 10 };
			Foo f2{ 20 };
			Foo f3{ 30 };
			Foo f4{ 40 };

			hash.Insert(f1, f2);
			hash.Insert(f2, f1);
			hash.Insert(f3, f4);
			hash.Insert(f4, f3);
			Assert::IsTrue(hash.ContainsKey(f1));
			Assert::IsTrue(hash.ContainsKey(f2));
			Assert::IsTrue(hash.ContainsKey(f3));
			Assert::IsTrue(hash.ContainsKey(f4));

			hash.Remove(f3);
			Assert::IsFalse(hash.ContainsKey(f3));
			hash.Remove(f4);
			Assert::IsFalse(hash.ContainsKey(f4));
			hash.Remove(f1);
			Assert::IsFalse(hash.ContainsKey(f1));
			hash.Remove(f2);
			Assert::IsFalse(hash.ContainsKey(f2));
		}

		TEST_METHOD(Clear)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{}, FooEqualityFunct{} };
			Assert::AreEqual(hash.Size(), std::size_t(0u));
			hash.Clear();
			Assert::AreEqual(hash.Size(), std::size_t(0u));

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);
			hash.Insert(f2, f1);
			Assert::AreEqual(hash.Size(), std::size_t(2u));

			hash.Clear();
			Assert::AreEqual(hash.Size(), std::size_t(0u));
		}

		TEST_METHOD(ReHash)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{} };

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);
			hash.Insert(f2, f1);

			Foo f3{ 10 };
			Foo f4{ 20 };
			Assert::IsTrue(hash.ContainsKey(f3));
			Assert::AreEqual(hash.At(f1), f4);

			Assert::IsTrue(hash.ReHash(17u));
			Assert::IsTrue(hash.ContainsKey(f3));
			Assert::AreEqual(hash.At(f1), f4);
			Assert::IsTrue(hash.ContainsKey(f2));
			Assert::AreEqual(hash.At(f2), f3);

			hash.ReHash(FooEqualityFunct{});
			Assert::IsTrue(hash.ContainsKey(f3));
			Assert::AreEqual(hash.At(f1), f4);
			Assert::IsTrue(hash.ContainsKey(f2));
			Assert::AreEqual(hash.At(f2), f3);

			hash.ReHash(FooHashFunctPrime{});
			Assert::IsTrue(hash.ContainsKey(f3));
			Assert::AreEqual(hash.At(f1), f4);
			Assert::IsTrue(hash.ContainsKey(f2));
			Assert::AreEqual(hash.At(f2), f3);

			hash.ReHash(FooHashFunctZero{});
			hash.ReHash(FooEqualityFunctError{});
			Assert::IsTrue(hash.ContainsKey(f3));
			Assert::AreEqual(hash.At(f1), f4);
			Assert::IsTrue(hash.ContainsKey(f2));	// because this equality funct returns true always
		}
#pragma endregion

#pragma region HASHMAP ITERATOR TEST
		TEST_METHOD(ITR_Constructor)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{} };
			Assert::AreEqual(hash.begin(), hash.end());

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);

			HashMap<Foo, Foo>::Const_Iterator citr{ hash.begin() };
			Assert::AreEqual(citr, hash.cbegin());

			hash.Insert(f2, f1);
			citr = hash.begin();
			Assert::AreEqual(citr, hash.cbegin());
		}

		TEST_METHOD(ITR_Equality)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{} };
			Assert::AreEqual(hash.begin(), hash.end());

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);
			Assert::AreNotEqual(hash.begin(), hash.end());

			HashMap<Foo, Foo>::Const_Iterator citr = hash.cbegin();
			Assert::AreEqual(citr, hash.cbegin());
			Assert::AreNotEqual(citr, hash.cend());
		}

		TEST_METHOD(ITR_Increment)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{} };

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);
			hash.Insert(f2, f1);

			for (HashMap<Foo, Foo>::Iterator itr = hash.begin(); itr != hash.end(); ++itr);
			for (HashMap<Foo, Foo>::Iterator itr = hash.begin(); itr != hash.end(); itr++);

			const HashMap<Foo, Foo> hashconst = hash;
			for (HashMap<Foo, Foo>::Const_Iterator itr = hashconst.begin(); itr != hashconst.end(); ++itr);
			for (HashMap<Foo, Foo>::Const_Iterator itr = hashconst.begin(); itr != hashconst.end(); itr++);
		}

		TEST_METHOD(ITR_Dereference)
		{
			HashMap<Foo, Foo> hash{ 11u, FooHashFunct{} };

			Foo f1{ 10 };
			Foo f2{ 20 };

			hash.Insert(f1, f2);

			Assert::AreEqual(*(hash.begin()), std::pair<const Foo, Foo>(f1, f2));
			Assert::AreEqual(hash.begin()->first, f1);
			Assert::AreEqual(hash.begin()->second, f2);

			Assert::AreEqual(*(hash.cbegin()), std::pair<const Foo, Foo>(f1, f2));
			Assert::AreEqual(hash.cbegin()->first, f1);
			Assert::AreEqual(hash.cbegin()->second, f2);
		}
#pragma endregion


	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState HashMapTests::_startMemState;
}
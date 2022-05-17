#include "pch.h"
#include "IEventSubscriber.h"
#include "IEventPublisher.h"
#include "Foo.h"
#include "EventQueue.h"
#include "Event.h"
#include "ToStringSpecializations.h"

using namespace std::chrono_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(EventTests)
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

		struct FooSubscriber final : public IEventSubscriber
		{
			void ReceiveEvent(const IEventPublisher& event) override
			{
				const Event<Foo>* fooevent = event.As<Event<Foo>>();
				if (fooevent == nullptr) return;

				data = fooevent->GetMessage().Data();
			}

			int data{ -1 };
		};

		struct Bar final
		{
			Bar() = default;
			int step{ 2 };
		};

		struct BarSubscriber final : public IEventSubscriber
		{
			void ReceiveEvent(const IEventPublisher& event) override
			{
				const Event<Bar>* barevent = event.As<Event<Bar>>();
				if (barevent == nullptr) return;

				data += barevent->GetMessage().step;
			}

			int data{ 0 };
		};

		struct QueueRef final
		{
			EventQueue& eventQueue;
			GameTime& gametime;
			QueueRef(EventQueue& eq, GameTime& gt) :eventQueue{ eq }, gametime{ gt } {}
		};

		struct QAddEventSubscriber final : public IEventSubscriber
		{
			void ReceiveEvent(const IEventPublisher& event) override
			{
				++_count;
				const Event<QueueRef>* qevent = event.As<Event<QueueRef>>();
				if (qevent == nullptr) return;

				QueueRef ref{ qevent->GetMessage().eventQueue, qevent->GetMessage().gametime };
				std::shared_ptr<Event<QueueRef>> qeventagain = std::make_shared<Event<QueueRef>>(ref);
				qevent->GetMessage().eventQueue.Enqueue(qeventagain, qevent->GetMessage().gametime);
			}

			int _count{ 0 };
		};

		struct QClearSubscriber final : public IEventSubscriber
		{
			void ReceiveEvent(const IEventPublisher& event) override
			{
				++_count;
				const Event<QueueRef>* qevent = event.As<Event<QueueRef>>();
				if (qevent == nullptr) return;

				qevent->GetMessage().eventQueue.Clear();
			}

			int _count{ 0 };
		};

		struct subscriber final
		{
			IEventSubscriber& sub;
			subscriber(IEventSubscriber& s) :sub{ s } {}
		};

		struct ChangeSubscriber final : public IEventSubscriber
		{
			void ReceiveEvent(const IEventPublisher& event) override
			{
				++_count;
				const Event<subscriber>* sevent = event.As<Event<subscriber>>();
				if (sevent == nullptr) return;

				Event<subscriber>::Unsubscribe(*this);
				Event<subscriber>::Subscribe(sevent->GetMessage().sub);
			}

			int _count{ 0 };
		};

		struct UnsubAllSubscriber final : public IEventSubscriber
		{
			void ReceiveEvent(const IEventPublisher& event) override
			{
				++_count;
				const Event<subscriber>* sevent = event.As<Event<subscriber>>();
				if (sevent == nullptr) return;

				Event<subscriber>::UnsubscribeAll();
			}

			int _count{ 0 };
		};

		TEST_METHOD(receive_event)
		{
			GameTime gametime;
			EventQueue eventQ;
			FooSubscriber foosub;
			FooSubscriber foosubagain;
			BarSubscriber barsub;
			BarSubscriber barsubagain;

			std::shared_ptr<Event<Foo>> fevent = std::make_shared<Event<Foo>>(Foo {10});
			Assert::AreEqual(fevent->GetMessage(), Foo{ 10 });

			std::shared_ptr<Event<Bar>> be = std::make_shared<Event<Bar>>(Bar{});

			Assert::AreEqual(eventQ.Size(), std::size_t(0u));
			eventQ.Enqueue(fevent, gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(1u));
			eventQ.Enqueue(be, gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(2u));

			Event<Foo>::Subscribe(foosub);
			Event<Foo>::Subscribe(barsub);
			Event<Foo>::Subscribe(barsubagain);
			Event<Bar>::Subscribe(foosub);
			Event<Bar>::Subscribe(foosubagain);
			Event<Bar>::Subscribe(barsub);
			Event<Bar>::Subscribe(barsub);

			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			Assert::AreEqual(foosub.data, -1);
			Assert::AreEqual(foosubagain.data, -1);
			Assert::AreEqual(barsub.data, 0);
			Assert::AreEqual(barsubagain.data, 0);
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, 10);
			Assert::AreEqual(foosubagain.data, -1);
			Assert::AreEqual(barsub.data, 2);
			Assert::AreEqual(barsubagain.data, 0);

			Event<Foo>::Subscribe(foosubagain);
			Event<Bar>::Subscribe(barsubagain);

			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, 10);
			Assert::AreEqual(foosubagain.data, -1);
			Assert::AreEqual(barsub.data, 2);
			Assert::AreEqual(barsubagain.data, 0);

			eventQ.Enqueue(fevent, gametime);
			eventQ.Enqueue(be, gametime);
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, 10);
			Assert::AreEqual(foosubagain.data, -1);
			Assert::AreEqual(barsub.data, 2);
			Assert::AreEqual(barsubagain.data, 0);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(2s));
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, 10);
			Assert::AreEqual(foosubagain.data, 10);
			Assert::AreEqual(barsub.data, 4);
			Assert::AreEqual(barsubagain.data, 2);

			// DELAY
			foosub.data = foosubagain.data = barsubagain.data = barsub.data = 0;
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point()); // THE BIG BANG OMGGGGGGGG

			eventQ.Enqueue(fevent, gametime, 1s);
			Assert::AreEqual(eventQ.GetEventInfo(fevent)->Delay(), 1000ms);
			Assert::AreEqual(eventQ.GetEventInfo(fevent)->TimeEnqueued(), std::chrono::high_resolution_clock::time_point());
			Assert::IsFalse(eventQ.GetEventInfo(fevent)->IsExpired(gametime));
			Assert::IsNull(eventQ.GetEventInfo(be));
			eventQ.Enqueue(be, gametime, 2s);
			Assert::AreEqual(eventQ.GetEventInfo(be)->Delay(), 2000ms);
			Assert::AreEqual(eventQ.GetEventInfo(be)->TimeEnqueued(), std::chrono::high_resolution_clock::time_point());
			Assert::IsFalse(eventQ.GetEventInfo(be)->IsExpired(gametime));

			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(500ms));
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, 0);
			Assert::AreEqual(foosubagain.data, 0);
			Assert::AreEqual(barsub.data, 0);
			Assert::AreEqual(barsubagain.data, 0);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, 0);
			Assert::AreEqual(foosubagain.data, 0);
			Assert::AreEqual(barsub.data, 0);
			Assert::AreEqual(barsubagain.data, 0);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(2s));
			Assert::IsTrue(eventQ.GetEventInfo(fevent)->IsExpired(gametime));
			Assert::IsFalse(eventQ.GetEventInfo(be)->IsExpired(gametime));
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, 10);
			Assert::AreEqual(foosubagain.data, 10);
			Assert::AreEqual(barsub.data, 0);
			Assert::AreEqual(barsubagain.data, 0);
			Event<Bar>::Unsubscribe(barsubagain);
			Event<Bar>::Unsubscribe(barsubagain);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(3s));
			Assert::IsTrue(eventQ.GetEventInfo(be)->IsExpired(gametime));
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, 10);
			Assert::AreEqual(foosubagain.data, 10);
			Assert::AreEqual(barsub.data, 2);
			Assert::AreEqual(barsubagain.data, 0);

			Assert::IsTrue(eventQ.IsEmpty());
			// CLEANUP
			Event<Foo>::CleanUp();
			Event<Bar>::CleanUp();
		}

		TEST_METHOD(clear)
		{
			GameTime gametime;
			EventQueue eventQ;
			FooSubscriber foosub;
			FooSubscriber foosubagain;
			std::shared_ptr<Event<Foo>> event10 = std::make_shared<Event<Foo>>(Foo {10});
			std::shared_ptr<Event<Foo>> event20 = std::make_shared<Event<Foo>>(Foo {20});

			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(0s));
			eventQ.Enqueue(event10, gametime);
			eventQ.Enqueue(event20, gametime);
			Event<Foo>::Subscribe(foosub);
			Event<Foo>::Subscribe(foosubagain);
			Assert::AreEqual(foosub.data, -1);
			Assert::AreEqual(foosubagain.data, -1);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(500ms));
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, 20);
			Assert::AreEqual(foosubagain.data, 20);

			eventQ.Enqueue(event10, gametime);
			eventQ.Enqueue(event20, gametime, 1s);
			foosub.data = foosubagain.data = -1;
			Assert::AreEqual(foosub.data, -1);
			Assert::AreEqual(foosubagain.data, -1);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			eventQ.Clear();
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, -1);
			Assert::AreEqual(foosubagain.data, -1);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(2s));
			eventQ.Update(gametime);
			Assert::AreEqual(foosub.data, -1);
			Assert::AreEqual(foosubagain.data, -1);

			Event<Foo>::CleanUp();
		}

		TEST_METHOD(recursive_enqueue)
		{
			GameTime gametime;
			EventQueue eventQ;
			QAddEventSubscriber qsub;

			QueueRef qref{ eventQ, gametime };
			std::shared_ptr<Event<QueueRef>> qevent = std::make_shared<Event<QueueRef>>(qref);
			FooSubscriber foosub;
			std::shared_ptr<Event<Foo>> event10 = std::make_shared<Event<Foo>>(Foo{ 10 });

			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(0s));
			Event<QueueRef>::Subscribe(qsub);
			Event<Foo>::Subscribe(foosub);

			Assert::IsTrue(eventQ.IsEmpty());
			eventQ.Enqueue(qevent, gametime);
			eventQ.Enqueue(qevent, gametime);
			eventQ.Enqueue(qevent, gametime);
			eventQ.Enqueue(event10, gametime,1s);
			eventQ.Enqueue(event10, gametime,1s);
			Assert::AreEqual(eventQ.Size(), std::size_t(5u));
			Assert::AreEqual(qsub._count, 0);
			Assert::AreEqual(foosub.data, -1);

			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			eventQ.Update(gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(5u));
			Assert::AreEqual(qsub._count, 3);
			Assert::AreEqual(foosub.data, -1);

			eventQ.Update(gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(5u));
			Assert::AreEqual(qsub._count, 3);
			Assert::AreEqual(foosub.data, -1);

			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(2s));
			eventQ.Update(gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(3u));
			Assert::AreEqual(qsub._count, 6);
			Assert::AreEqual(foosub.data, 10);

			eventQ.Update(gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(3u));
			Assert::AreEqual(qsub._count, 6);
			Assert::AreEqual(foosub.data, 10);

			QClearSubscriber qclearsub;
			Event<QueueRef>::Subscribe(qclearsub);
			Assert::AreEqual(qclearsub._count, 0);

			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(3s));
			eventQ.Update(gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(3u));
			Assert::AreEqual(qsub._count, 9);
			Assert::AreEqual(foosub.data, 10);
			Assert::AreEqual(qclearsub._count, 3);

			eventQ.Update(gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(3u));
			Assert::AreEqual(qsub._count, 9);
			Assert::AreEqual(foosub.data, 10);
			Assert::AreEqual(qclearsub._count, 3);

			eventQ.Clear();
			Assert::IsTrue(eventQ.IsEmpty());
			eventQ.Update(gametime);
			Assert::IsTrue(eventQ.IsEmpty());
			Assert::AreEqual(qsub._count, 9);
			Assert::AreEqual(foosub.data, 10);
			Assert::AreEqual(qclearsub._count, 3);

			Event<QueueRef>::CleanUp();
			Event<Foo>::CleanUp();
		}

		TEST_METHOD(change_subscriber)
		{
			GameTime gametime;
			EventQueue eventQ;
			ChangeSubscriber chsub;
			UnsubAllSubscriber unsub;
			std::shared_ptr<Event<subscriber>> subevent = std::make_shared<Event<subscriber>>(unsub);

			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(0s));
			Event<subscriber>::Subscribe(chsub);

			Assert::IsTrue(eventQ.IsEmpty());
			eventQ.Enqueue(subevent, gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(1u));
			Assert::AreEqual(chsub._count, 0);
			Assert::AreEqual(unsub._count, 0);

			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			eventQ.Update(gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(0u));
			Assert::AreEqual(chsub._count, 1);
			Assert::AreEqual(unsub._count, 0);

			eventQ.Enqueue(subevent, gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(1u));
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(2s));
			eventQ.Update(gametime);
			Assert::AreEqual(eventQ.Size(), std::size_t(0u));
			Assert::AreEqual(chsub._count, 1);
			Assert::AreEqual(unsub._count, 1);

			eventQ.Enqueue(subevent, gametime);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(3s));
			eventQ.Update(gametime);
			Assert::AreEqual(chsub._count, 1);
			Assert::AreEqual(unsub._count, 1);

			eventQ.Enqueue(subevent, gametime);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(4s));
			eventQ.Update(gametime);
			Assert::AreEqual(chsub._count, 1);
			Assert::AreEqual(unsub._count, 1);

			Event<subscriber>::CleanUp();
		}

		TEST_METHOD(copy_semantics)
		{
			Foo foo10{ 10 };
			Foo foo20{ 20 };

			{
				Event<Foo> fevent{ foo10 };
				Event<Foo> feventagain{ fevent };
				Assert::AreNotSame(fevent.GetMessage(), feventagain.GetMessage());
				Assert::AreEqual(fevent.GetMessage(), feventagain.GetMessage());
			}

			{
				Event<Foo> fevent{ foo10 };
				Event<Foo> feventagain{ foo20 };
				feventagain = fevent;
				Assert::AreNotSame(fevent.GetMessage(), feventagain.GetMessage());
				Assert::AreEqual(fevent.GetMessage(), feventagain.GetMessage());
			}

			{
				EventQueue q;
				q.Enqueue(std::make_shared<Event<Bar>>(Bar{}), GameTime{});
				EventQueue qagain = q;
				Assert::AreEqual(q.Size(), qagain.Size());
			}

			{
				EventQueue q;
				q.Enqueue(std::make_shared<Event<Bar>>(Bar{}), GameTime{});
				EventQueue qagain;
				qagain = q;
				Assert::AreEqual(q.Size(), qagain.Size());
			}
		}

		TEST_METHOD(move_semantics)
		{
			Foo foo10{ 10 };
			Foo foo20{ 20 };

			{
				Event<Foo> fevent(foo10);
				Event<Foo> feventagain(std::move(fevent));
				Assert::IsFalse(&fevent.GetMessage() == &feventagain.GetMessage());
			}

			{
				Event<Foo> fevent(foo10);
				Event<Foo> feventagain(foo20);
				feventagain = std::move(fevent);
				Assert::IsFalse(&fevent.GetMessage() == &feventagain.GetMessage());
			}

			{
				EventQueue q;
				q.Enqueue(std::make_shared<Event<Bar>>(Bar{}), GameTime{});
				EventQueue qagain = std::move(q);
				Assert::AreEqual(qagain.Size(), std::size_t(1));
			}

			{
				EventQueue q;
				q.Enqueue(std::make_shared<Event<Bar>>(Bar{}), GameTime{});
				EventQueue qagain;
				qagain = std::move(q);
				Assert::AreEqual(qagain.Size(), std::size_t(1));
			}
		}

		TEST_METHOD(rtti_event)
		{
			Foo foo10{ 10 };
			Event<Foo> fevent(foo10);

			RTTI* rtti = &fevent;
			Assert::IsTrue(rtti->Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(rtti->Is(IEventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));

			Event<Foo>* e = rtti->As<Event<Foo>>();
			Assert::IsNotNull(e);
			Assert::AreEqual(&fevent, e);

			IEventPublisher* ep = rtti->As<IEventPublisher>();
			Assert::IsNotNull(ep);
			Assert::AreEqual(&fevent, static_cast<Event<Foo>*>(ep));
			Assert::IsTrue(rtti->Is(ep->TypeIdInstance()));

			Event<Foo>::CleanUp();
		}

		TEST_METHOD(rtti_publisher)
		{
			Foo foo10{ 10 };
			Event<Foo> fevent(foo10);
			IEventPublisher* eventP = static_cast<IEventPublisher*>(&fevent);

			RTTI* rtti = eventP;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(IEventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(eventP->TypeIdInstance(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			IEventPublisher* a = rtti->As<IEventPublisher>();
			Assert::IsNotNull(a);
			Assert::AreEqual(eventP, a);
		}

	private:
		static _CrtMemState _startMemState;

	};

	_CrtMemState EventTests::_startMemState;
}
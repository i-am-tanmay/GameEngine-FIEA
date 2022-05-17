#include "pch.h"
#include "SList.h"
#include "Stack.h"
#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"
#include "JsonParseManager.h"
#include "Factory.h"
#include "IEventSubscriber.h"
#include "Event.h"
#include "EventQueue.h"

using namespace FIEAGameEngine;



struct Explosion
{
	std::size_t _index{ 999 };
	Explosion(size_t i) :_index{ i } {};
	bool operator==(const Explosion& e) { return _index == e._index; }
};

struct FireFade
{
	std::size_t _index{ 999 };
	FireFade(size_t i) :_index{ i } {};
	bool operator==(const FireFade& f) { return _index == f._index; }
};

class TileWrapper final : public FIEAGameEngine::IEventSubscriber
{
public:
	TileWrapper()
	{
		Event<Explosion>::Subscribe(*this);
		Event<FireFade>::Subscribe(*this);
	}

	void ReceiveEvent(const IEventPublisher& event) override
	{

		const Event<Explosion>* exp = event.As<Event<Explosion>>();
		if (exp != nullptr)
		{
			x = exp->GetMessage()._index;
		}

		const Event<FireFade>* exp2 = event.As<Event<FireFade>>();
		if (exp2 != nullptr)
		{
			x = 500;
		}

	}

	size_t x{ 0 };
};

int main()
{
	TileWrapper tile;

	EventQueue q;
	GameTime gametime;

	std::shared_ptr<Event<Explosion>> eevent = std::make_shared<Event<Explosion>>(Explosion{ size_t(10) });

	gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(0s));
	q.Enqueue(eevent, gametime);

	gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
	q.Update(gametime);

	tile.x;
}
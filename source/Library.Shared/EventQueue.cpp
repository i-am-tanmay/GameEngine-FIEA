#include "pch.h"
#include "EventQueue.h"
#include "GameTime.h"
#include "IEventPublisher.h"
#include <algorithm>

namespace FIEAGameEngine
{
	void EventQueue::Enqueue(std::shared_ptr<IEventPublisher> event_, const GameTime& gametime, std::chrono::milliseconds delay)
	{
		if (!isUpdateRunning)_events.PushBack(EventInfo{ event_, gametime, delay });
		else _pendingEvents.PushBack(EventInfo{ event_, gametime, delay });
	}

	void EventQueue::Update(const GameTime& gametime)
	{
		isUpdateRunning = true;
		isClearPending = false;

		_events.Reserve(_events.Size() + _pendingEvents.Size());
		for (EventInfo& event : _pendingEvents) _events.PushBack(std::move(event));
		_pendingEvents.Clear();

		Vector<EventInfo>::Iterator itr = std::partition(_events.begin(), _events.end(), [gametime](EventInfo& event) {return !(event.IsExpired(gametime)); });

		for (Vector<EventInfo>::Iterator itr_deliver = itr; itr_deliver != _events.end(); ++itr_deliver)
		{
			(*itr_deliver).event->Deliver();
		}

		if (!isClearPending)_events.RemoveAll(itr, _events.end());

		isUpdateRunning = false;
		if (isClearPending)
		{
			_events.Clear();
			isClearPending = false;
		}
	}

	void EventQueue::Clear()
	{
		if (!isUpdateRunning)
		{
			_events.Clear();
			_pendingEvents.Clear();
		}
		else isClearPending = true;
	}

	bool EventQueue::IsEmpty() const { return _events.IsEmpty() && _pendingEvents.IsEmpty(); }
	EventQueue::size_type EventQueue::Size() const { return _events.Size() + _pendingEvents.Size(); }

	const EventInfo* EventQueue::GetEventInfo(std::shared_ptr<IEventPublisher> event_) const
	{
		for (Vector<EventInfo>::Const_Iterator itr = _events.cbegin(); itr != _events.cend(); ++itr)
		{
			if (event_ == (*itr).event) return &(*itr);
		}

		return nullptr;
	}
}
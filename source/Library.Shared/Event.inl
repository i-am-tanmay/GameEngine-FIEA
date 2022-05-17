#include "Event.h"

namespace FIEAGameEngine
{
	template <typename MessageType>
	RTTI::IdType Event<MessageType>::TypeIdClass() { return _typeId; }

	template <typename MessageType>
	RTTI::IdType Event<MessageType>::TypeIdInstance() const { return TypeIdClass(); }

	template <typename MessageType>
	bool Event<MessageType>::Is(RTTI::IdType id) const { return (id == _typeId ? true : IEventPublisher::Is(id)); }

	template <typename MessageType>
	const RTTI::IdType Event<MessageType>::_typeId = reinterpret_cast<RTTI::IdType>(&Event<MessageType>::_typeId);

	template<typename MessageType>
	Event<MessageType>::Event(const MessageType& msg) : IEventPublisher{ _subscribers }, _message{ msg } {}

	template<typename MessageType>
	Event<MessageType>::Event(MessageType&& msg) : IEventPublisher{ _subscribers }, _message{ std::move(msg) } {}

	template <typename MessageType>
	Event<MessageType>& Event<MessageType>::operator=(const Event<MessageType>& rhs)
	{
		if (this != &rhs)
		{
			CleanUp();

			_message = rhs._message;
			_subscribers = rhs._subscribers;
			_pendingSubscribers = rhs._pendingSubscribers;
			isDeliverRunning = rhs.isDeliverRunning;
			isUnsubscribeAllCalled = rhs.isUnsubscribeAllCalled;
		}

		return *this;
	}

	template <typename MessageType>
	Event<MessageType>& Event<MessageType>::operator=(Event<MessageType>&& rhs) noexcept
	{
		if (this != &rhs)
		{
			CleanUp();

			_message = std::move(rhs._message);
			_subscribers = std::move(rhs._subscribers);
			_pendingSubscribers = std::move(rhs._pendingSubscribers);
			isDeliverRunning = rhs.isDeliverRunning;
			isUnsubscribeAllCalled = rhs.isUnsubscribeAllCalled;
		}

		return *this;
	}

	template<typename MessageType>
	void Event<MessageType>::Subscribe(IEventSubscriber& subscriber)
	{
		if (!isDeliverRunning)
		{
			for (Vector<std::reference_wrapper<IEventSubscriber>>::Iterator itr = _subscribers.begin(); itr != _subscribers.end(); ++itr)
				if (&subscriber == &((*itr).get())) return;


			_subscribers.PushBack(subscriber) != _subscribers.end();
		}
		else
		{
			_pendingSubscribers.PushBack(std::pair<IEventSubscriber&, bool>(subscriber, true));
		}
	}

	template<typename MessageType>
	void Event<MessageType>::Unsubscribe(IEventSubscriber& subscriber)
	{
		if (!isDeliverRunning)
		{
			for (Vector<std::reference_wrapper<IEventSubscriber>>::Iterator itr = _subscribers.begin(); itr != _subscribers.end(); ++itr)
			{
				if (&subscriber == &((*itr).get()))
				{
					_subscribers.Remove(itr);
					return;
				}
			}
		}
		else
		{
			_pendingSubscribers.PushBack(std::pair<IEventSubscriber&, bool>(subscriber, false));
		}
	}

	template<typename MessageType>
	void Event<MessageType>::UnsubscribeAll()
	{
		if (!isDeliverRunning) _subscribers.Clear();
		else isUnsubscribeAllCalled = true;
	}

	template<typename MessageType>
	void Event<MessageType>::PreDeliver()
	{
		isDeliverRunning = true;
		isUnsubscribeAllCalled = false;
	}

	template<typename MessageType>
	void Event<MessageType>::PostDeliver()
	{
		isDeliverRunning = false;

		if (isUnsubscribeAllCalled) UnsubscribeAll();

		for (std::pair<IEventSubscriber&, bool>& pair : _pendingSubscribers)
		{
			if (pair.second) Subscribe(pair.first);
			else if(!isUnsubscribeAllCalled) Unsubscribe(pair.first);
		}

		_pendingSubscribers.Clear();
		isUnsubscribeAllCalled = false;
	}

	template<typename MessageType>
	const MessageType& Event<MessageType>::GetMessage() const
	{
		return _message;
	}

	template<typename MessageType>
	void Event<MessageType>::CleanUp()
	{
		_subscribers.Clear();
		_subscribers.ShrinkToFit();

		_pendingSubscribers.Clear();
		_pendingSubscribers.ShrinkToFit();
	}
}
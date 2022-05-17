#pragma once

namespace FIEAGameEngine
{
	class IEventPublisher;

	class IEventSubscriber
	{
	public:

		/// <summary>
		/// Get Notified about an event you are subscribed to
		/// </summary>
		/// <param name="event"></param>
		virtual void ReceiveEvent(const IEventPublisher& event) = 0;

		virtual ~IEventSubscriber() = default;
	};
}
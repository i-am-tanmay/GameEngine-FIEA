#pragma once
#include "IEventPublisher.h"

namespace FIEAGameEngine
{
	template <typename MessageType>
	class Event final : public IEventPublisher
	{
		RTTI_DECLARATIONS(Event);

	public:

		/// <summary>
		/// Constructor for Event : copy construct message
		/// </summary>
		/// <param name="msg">Reference to message to copy</param>
		Event(const MessageType& msg);

		/// <summary>
		/// Constructor for Event : move construct message
		/// </summary>
		/// <param name="msg">Reference to message to move</param>
		Event(MessageType&& msg);

		/// <summary>
		/// Copy Construct this event
		/// </summary>
		/// <param name="rhs">Reference to other event to copy</param>
		Event(const Event& rhs) =default;

		/// <summary>
		/// Move Construct this event
		/// </summary>
		/// <param name="rhs">Reference to other event to move</param>
		Event(Event&& rhs) noexcept=default;

		/// <summary>
		/// Copy Assignment for Event
		/// </summary>
		/// <param name="rhs">Reference to other event to copy</param>
		/// <returns>Reference to new event</returns>
		Event& operator=(const Event& rhs);

		/// <summary>
		/// Move Assignment for Event
		/// </summary>
		/// <param name="rhs">Reference to other event to move</param>
		/// <returns>Reference to new event</returns>
		Event& operator=(Event&& rhs) noexcept;

		/// <summary>
		/// Add a new subscriber to this event type
		/// </summary>
		/// <param name="subscriber">eference to new subscriber</param>
		static void Subscribe(IEventSubscriber& subscriber);

		/// <summary>
		/// Unsubscribe a subscriber
		/// </summary>
		/// <param name="subscriber">Reference to subscriber to unsubscribe</param>
		static void Unsubscribe(IEventSubscriber& subscriber);

		/// <summary>
		/// Unsibscrive all subscribers to this event type
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Get const Message of this event
		/// </summary>
		/// <returns></returns>
		const MessageType& GetMessage() const;

		/// <summary>
		/// CleanUp all static vectors (for testing)
		/// </summary>
		static void CleanUp();

	protected:
		void PreDeliver() override;
		void PostDeliver() override;

	private:
		MessageType _message;
		inline static Vector<std::reference_wrapper<IEventSubscriber>> _subscribers;

		inline static bool isDeliverRunning{ false };
		inline static Vector<std::pair<IEventSubscriber&, bool>> _pendingSubscribers;
		inline static bool isUnsubscribeAllCalled{ false };
	};
}

#include "Event.inl"
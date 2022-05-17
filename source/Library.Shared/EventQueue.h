#pragma once
#include "Vector.h"
#include "GameClock.h"
#include "GameTime.h"

using namespace std::chrono_literals;
namespace FIEAGameEngine
{
	class IEventPublisher;

	struct EventInfo final
	{
		/// <summary>
		/// Constructor for EventInfo
		/// </summary>
		/// <param name="event_">Shared Pointer to event to Queue</param>
		/// <param name="gametime_">Reference to gametime to get CurrentTime</param>
		/// <param name="delay_">Delay for the event</param>
		EventInfo(std::shared_ptr<IEventPublisher> event_, const GameTime& gametime_, std::chrono::milliseconds delay_) : event{ event_ }, endtime{ gametime_.CurrentTime() + delay_ }, delay{ delay_ }{}

		/// <summary>
		/// Time when this event was queued
		/// </summary>
		/// <returns>Timepoint when the event was enqueued</returns>
		std::chrono::high_resolution_clock::time_point TimeEnqueued() const { return endtime - delay; }

		/// <summary>
		/// Delay for this event
		/// </summary>
		/// <returns>Milliseconds delay for this queued event</returns>
		std::chrono::milliseconds Delay() const { return delay; }

		/// <summary>
		/// Is this event expired
		/// </summary>
		/// <param name="gametime">Reference to GameTime to get current time</param>
		/// <returns>TRUE if current time is more than time enqueued + delay | FALSE if not</returns>
		bool IsExpired(const GameTime& gametime) const { return endtime < gametime.CurrentTime(); }

		friend class EventQueue;

	private:
		std::shared_ptr<IEventPublisher> event;
		std::chrono::high_resolution_clock::time_point endtime;
		std::chrono::milliseconds delay;
	};

	class EventQueue final
	{
	public:
		using size_type = std::size_t;

	public:

		/// <summary>
		/// Constructor for EventQueue
		/// </summary>
		EventQueue() = default;

		/// <summary>
		/// Copy Construct an event queue
		/// </summary>
		/// <param name="rhs">Reference to other event queue for copy</param>
		EventQueue(const EventQueue& rhs) = default;

		/// <summary>
		/// Copy Assignment : Event queue
		/// </summary>
		/// <param name="rhs">Reference to other event queue for copy</param>
		/// <returns>Reference to new event queue</returns>
		EventQueue& operator=(const EventQueue& rhs) = default;

		/// <summary>
		/// Move Construct Event queue
		/// </summary>
		/// <param name="rhs">Reference to other event queue for move</param>
		EventQueue(EventQueue&& rhs) noexcept = default;

		/// <summary>
		/// Move Assignment Event queue
		/// </summary>
		/// <param name="rhs">Reference to other event queue for move</param>
		/// <returns>Reference to new event queue</returns>
		EventQueue& operator=(EventQueue&& rhs) noexcept = default;

		/// <summary>
		/// Queue a new event
		/// </summary>
		/// <param name="event_">Shared Pointer to event to be Queued</param>
		/// <param name="gametime">Reference to GameTime for current time</param>
		/// <param name="delay">Delay for the event</param>
		void Enqueue(std::shared_ptr<IEventPublisher> event_, const GameTime& gametime, std::chrono::milliseconds delay = 0ms);

		/// <summary>
		/// Call Deliver on every event in the queue
		/// </summary>
		/// <param name="gametime">Reference to GameTime to get current time</param>
		void Update(const GameTime& gametime);

		/// <summary>
		/// CLear the current queue of events
		/// </summary>
		void Clear();

		/// <summary>
		/// Are there any events queued
		/// </summary>
		/// <returns>TRUE if the queue is empty | FALSE if not</returns>
		bool IsEmpty() const;

		/// <summary>
		/// How many events are in the queue
		/// </summary>
		/// <returns>Count of events in the queue</returns>
		size_type Size() const;

		/// <summary>
		/// Get Info about a certain event from the queue (first instance in the queue)
		/// </summary>
		/// <param name="event_">Shared Pointer to event to find info about</param>
		/// <returns>Struct containing info on the event</returns>
		const EventInfo* GetEventInfo(std::shared_ptr<IEventPublisher> event_) const;

	private:
		Vector<EventInfo> _events;
		Vector<EventInfo> _pendingEvents;

		bool isUpdateRunning{ false };
		bool isClearPending{ false };
	};
}
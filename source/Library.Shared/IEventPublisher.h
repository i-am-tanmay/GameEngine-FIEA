#pragma once
#include "RTTI.h"
#include "Vector.h"

namespace FIEAGameEngine
{
	class IEventSubscriber;

	class IEventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(IEventPublisher);

	public:
		/// <summary>
		/// Deliver event to all the subscribers
		/// </summary>
		void Deliver();

		virtual ~IEventPublisher() = default;

	protected:
		IEventPublisher(Vector<std::reference_wrapper<IEventSubscriber>>& subscribers);

		IEventPublisher(const IEventPublisher&) = default;
		IEventPublisher& operator=(const IEventPublisher&) = default;
		IEventPublisher(IEventPublisher&&) noexcept = default;
		IEventPublisher& operator=(IEventPublisher&&) noexcept = default;

		virtual void PreDeliver() {};
		virtual void PostDeliver() {};

	private:
		Vector<std::reference_wrapper<IEventSubscriber>>& ref_subsribers;
	};
}
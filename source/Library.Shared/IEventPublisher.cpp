#include "pch.h"
#include "IEventPublisher.h"
#include "IEventSubscriber.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(IEventPublisher, RTTI);

	IEventPublisher::IEventPublisher(Vector<std::reference_wrapper<IEventSubscriber>>& subscribers) : ref_subsribers{ subscribers }
	{ }

	void IEventPublisher::Deliver()
	{
		PreDeliver();

		for (IEventSubscriber& sub : ref_subsribers)
		{
			sub.ReceiveEvent(*this);
		}

		PostDeliver();
	}
}
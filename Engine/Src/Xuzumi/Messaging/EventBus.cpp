#include "Xuzumi/Messaging/EventBus.hpp"

namespace Xuzumi
{
	EventSubscription::EventSubscription(
		TypeID eventTypeID,
		Internal::EventHandlerID handlerID
	)
		: mEventTypeID(eventTypeID)
		, mHandlerID(handlerID)
	{
	}

	TypeID EventSubscription::GetEventTypeID() const
	{
		return mEventTypeID;
	}

	Internal::EventHandlerID EventSubscription::GetHandlerID() const
	{
		return mHandlerID;
	}

	void EventBus::Unsubscribe(EventSubscription subscription)
	{
		TypeID eventTypeID = subscription.GetEventTypeID();
		Internal::EventHandlerID handlerID = subscription.GetHandlerID();
	
		auto routerIt = mDispatchers.find(eventTypeID);
		if (mDispatchers.end() != routerIt)
		{
			routerIt->second->RemoveHandler(handlerID);
		}		
	}

	void EventBus::Dispatch()
	{
		for (auto& router : mDispatchers)
		{
			router.second->Dispatch();
		}
	}
}

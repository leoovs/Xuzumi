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
	
		auto dispatcherIt = mDispatchers.find(eventTypeID);
		if (mDispatchers.end() != dispatcherIt)
		{
			dispatcherIt->second->RemoveHandler(handlerID);
		}		
	}

	void EventBus::UnsubscribeAll()
	{
		for (auto& dispatcher : mDispatchers)
		{
			dispatcher.second->RemoveAllHandlers();
		}
	}

	void EventBus::Dispatch()
	{
		for (auto& dispatcher : mDispatchers)
		{
			dispatcher.second->Dispatch();
		}
	}

	void EventBus::FlushEvents()
	{
		for (auto& dispatcher : mDispatchers)
		{
			dispatcher.second->FlushEvents();
		}
	}
}

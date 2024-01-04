#include "Xuzumi/Messaging/EventPublisher.hpp"

namespace Xuzumi
{
	void EventPublisher::Connect(ObserverPtr<EventBus> bus)
	{
		mBus = bus;
	}
}

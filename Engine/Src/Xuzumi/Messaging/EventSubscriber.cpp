#include "Xuzumi/Messaging/EventSubscriber.hpp"

namespace Xuzumi
{
	EventSubscriber::~EventSubscriber()
	{
		Unsubscribe();
	}

	EventSubscriber& EventSubscriber::Subscribe(ObserverPtr<EventBus> bus)
	{
		Unsubscribe();
		mBus = bus;

		return *this;
	}

	void EventSubscriber::Unsubscribe()
	{
		if (!mBus)
		{
			mSubscriptions.clear();
			return;
		}

		for (EventSubscription& subscription : mSubscriptions)
		{
			mBus->Unsubscribe(subscription);
		}

		mSubscriptions.clear();
		mBus = nullptr;
	}

	EventBus& EventSubscriber::GetBus() const
	{
		return *mBus;
	}

	void EventSubscriber::AddSubscription(EventSubscription subscription)
	{
		mSubscriptions.push_back(subscription);
	}
}

#include "Xuzumi/Messaging/EventSubscriber.hpp"

namespace Xuzumi
{
	EventSubscriber::~EventSubscriber()
	{
		Unsubscribe();
	}

	EventSubscriber& EventSubscriber::Subscribe(EventBus& bus)
	{
		Unsubscribe();
		mBus.Reset(&bus);

		return *this;
	}

	EventBus& EventSubscriber::GetBus() const
	{
		return *mBus;
	}

	void EventSubscriber::AddSubscription(EventSubscription subscription)
	{
		mSubscriptions.push_back(subscription);
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
}

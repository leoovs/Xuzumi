#pragma once

#include <Xuzumi/Xuzumi.hpp>

class App
{
public:
	App(Xuzumi::ObserverPtr<Xuzumi::EventBus> events);

private:
	Xuzumi::ObserverPtr<Xuzumi::EventBus> mEvents;
	Xuzumi::EventSubscriber mEventSubscriber;
};

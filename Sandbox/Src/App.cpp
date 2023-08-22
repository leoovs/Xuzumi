#include "App.hpp"

#include "Entity.hpp"

App::App(Xuzumi::ObserverPtr<Xuzumi::EventBus> events)
	: mEvents(events)
{
	mEventSubscriber.Subscribe(mEvents);

	mEventSubscriber
		.BeginThis(this)
		// Add methods here...
		.EndThis();

	XZ_LOG(Debug, "Nothing here...");
}

#include "App.hpp"

#include "Entity.hpp"

App::App(Xuzumi::ObserverPtr<Xuzumi::EventBus> events)
	: mEvents(events)
{
	// TODO: event subscriber should take
	// observer ptr instead of a reference.
	mEventSubscriber.Subscribe(*mEvents);

	mEventSubscriber
		.BeginThis(this)
		// Add methods here...
		.EndThis();

	XZ_LOG(Debug, "Nothing here...");
}

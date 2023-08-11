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
	
	auto p1 = Xuzumi::MakeUnique<Entity>(Xuzumi::IDType(105));
	auto p2 = Xuzumi::MakeUnique<Entity>(Xuzumi::IDType(501));

	p1->Display();
	p2->Display();

	p1.Swap(p2);
	p1 = p2.MimicrySafe<Entity>();

	p1->Display();
	*p2;
}

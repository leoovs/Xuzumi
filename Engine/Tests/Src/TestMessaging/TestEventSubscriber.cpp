#include <doctest/doctest.h>

#include "Xuzumi/Messaging/EventSubscriber.hpp"

TEST_CASE("Xuzumi::EventSubscriber")
{
	SUBCASE("Basic functionality")
	{
		struct MockEvent : public Xuzumi::Event<MockEvent> {};

		int handlerCounter = 0;

		auto handler = [&handlerCounter](const MockEvent& event)
		{
			handlerCounter++;
			return true;
		};

		Xuzumi::ObserverPtr<Xuzumi::EventBus> bus(new Xuzumi::EventBus);
		bus->Publish<MockEvent>();
		bus->Dispatch();
		CHECK(0 == handlerCounter);
		bus->FlushEvents();

		{
			Xuzumi::EventSubscriber sub;

			sub.Subscribe(bus)
				.Functor<MockEvent>(handler);

			bus->Publish<MockEvent>();	
			bus->Dispatch();
			CHECK(1 == handlerCounter);
		}

		bus->Publish<MockEvent>();
		bus->Dispatch();
		CHECK(1 == handlerCounter);

		delete bus.Release();
	}
}

#include <doctest/doctest.h>

#include "Xuzumi/Messaging/EventPublisher.hpp"

TEST_CASE("Xuzumi::EventPublisher")
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

		Xuzumi::EventBus bus;
		bus.Subscribe<MockEvent>(handler);

		Xuzumi::EventPublisher publisher;
		publisher.Connect(Xuzumi::ObserverPtr<Xuzumi::EventBus>(&bus));
		publisher.Publish<MockEvent>();
		CHECK(0 == handlerCounter);

		bus.Dispatch();
		CHECK(1 == handlerCounter);

		publisher.Connect(nullptr);
		publisher.Publish<MockEvent>();
		bus.Dispatch();
		CHECK(1 == handlerCounter);
	}
}

#include <doctest/doctest.h>

#include "Xuzumi/Messaging/EventBus.hpp"

TEST_CASE("Xuzumi::EventBus")
{
	SUBCASE("Basic functionality")
	{
		struct MockEvent : public Xuzumi::Event<MockEvent>
		{
			int MockData = 0;
			MockEvent(int data) : MockData(data) {}
		};

		Xuzumi::EventBus bus;
		std::vector<int> orderStorage;	

		auto handler = [&orderStorage](const MockEvent& event) -> bool
		{
			orderStorage.push_back(event.MockData);
			return true;	
		};

		bus.Publish<MockEvent>(1);
		
		bus.Dispatch();
		CHECK(0 == orderStorage.size());

		Xuzumi::EventSubscription sub = bus.Subscribe<MockEvent>(handler);
		CHECK(sub.GetEventTypeID() == Xuzumi::GetTypeID<MockEvent>());
		CHECK(sub.GetHandlerID() == 0);

		bus.Dispatch();
		CHECK(1 == orderStorage.size());
		CHECK(1 == orderStorage[0]);

		orderStorage.clear();
		bus.Publish<MockEvent>(4);
		bus.Publish<MockEvent>(36);
		bus.Dispatch();

		CHECK(orderStorage.size() == 2);
		CHECK(36 == orderStorage.front());
		CHECK(4 == orderStorage[1]);

		orderStorage.clear();
		bus.Unsubscribe(sub);
		bus.Dispatch();
		CHECK(orderStorage.size() == 0);
	}

	SUBCASE("Deferred dispatch")
	{
		struct MockEvent
		{
			int Number = 0;
			MockEvent(int number) : Number(number) {}
		};

		int evensCounter = 0;
		int oddsCounter = 0;

		auto handleEvens = [&evensCounter](const MockEvent& event)
		{
			if (event.Number % 2)
			{
				return false;
			}
			evensCounter++;
			return true;
		};

		auto handleOdds = [&oddsCounter](const MockEvent& event)
		{
			if (event.Number % 2)
			{
				oddsCounter++;
				return true;
			}
			return false;
		};

		Xuzumi::EventBus bus;

		bus.Publish<MockEvent>(1);
		bus.Publish<MockEvent>(2);
		bus.Publish<MockEvent>(3);

		bus.Subscribe<MockEvent>(handleEvens);
		bus.Subscribe<MockEvent>(handleOdds);
		bus.Dispatch();

		CHECK(1 == evensCounter);
		CHECK(2 == oddsCounter);

		bus.Publish<MockEvent>(5);
		bus.Publish<MockEvent>(1022);
		bus.FlushEvents();
		bus.Dispatch();

		CHECK(1 == evensCounter);
		CHECK(2 == oddsCounter);
	}

	SUBCASE("Multiple subscriptions")
	{
		struct MockEvent : public Xuzumi::Event<MockEvent> {};

		Xuzumi::EventBus bus;
		int counter = 0;

		Xuzumi::EventSubscription s1 = bus.Subscribe<MockEvent>(
			[&counter](const MockEvent& e)
			{
				counter++;
				return false;
			}
		);

		Xuzumi::EventSubscription s2 = bus.Subscribe<MockEvent>(
			[&counter](const MockEvent& e)
			{
				counter++;
				return false;
			}
		);

		bus.Publish<MockEvent>();
		bus.Dispatch();
		CHECK(2 == counter);

		bus.Dispatch();
		CHECK(4 == counter);

		bus.UnsubscribeAll();
		bus.Dispatch();
		CHECK(4 == counter);
	}
}

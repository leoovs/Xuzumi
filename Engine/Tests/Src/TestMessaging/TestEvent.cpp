#include <doctest/doctest.h>

#include "Xuzumi/Messaging/Event.hpp"

TEST_CASE("Xuzumi::Event<DerivedT>")
{
	SUBCASE("Type information")
	{
		struct MockEvent : public Xuzumi::Event<MockEvent> {};

		std::unique_ptr<Xuzumi::IEvent> event;
		event = std::make_unique<MockEvent>();

		CHECK(event->GetTypeInfo() == Xuzumi::TypeInfo::Get<MockEvent>());
	}
}

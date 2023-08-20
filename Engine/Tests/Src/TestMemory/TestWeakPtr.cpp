#include <doctest/doctest.h>

#include "Xuzumi/Memory/WeakPtr.hpp"

TEST_CASE("Xuzumi::WeakPtr<T>")
{
	SUBCASE("Weak referencing")
	{
		static int sInstanceCounter;

		struct MockResource
		{
			MockResource()
			{
				sInstanceCounter++;
			}

			~MockResource()
			{
				sInstanceCounter--;
			}
		};

		Xuzumi::SharedPtr<MockResource> strong;
		strong = Xuzumi::MakeShared<MockResource>();

		Xuzumi::WeakPtr<MockResource> weak = strong;

		CHECK(weak.UseCount() == strong.UseCount());
		CHECK(weak.UseCount() == 1);
		CHECK_FALSE(weak.Expired());
		CHECK(bool(weak.Lock()));
		CHECK(weak.Lock().Get() == strong.Get());

		strong.Reset();

		CHECK(weak.UseCount() == strong.UseCount());
		CHECK(weak.UseCount() == 0);
		CHECK(weak.Expired());
		CHECK_FALSE(bool(weak.Lock()));
	}	
}

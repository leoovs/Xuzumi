#include <doctest/doctest.h>

#include "Xuzumi/Memory/ObserverPtr.hpp"

TEST_CASE("Xuzumi::ObserverPtr<T>")
{
	SUBCASE("Basic functionality")
	{
		struct MockResource {};

		MockResource resource;
		Xuzumi::ObserverPtr<MockResource> ptr(&resource);

		CHECK(bool(ptr));
		CHECK(ptr.Get() == &resource);
		CHECK(ptr.Release() == &resource);
		CHECK_FALSE(bool(ptr));
	}
}

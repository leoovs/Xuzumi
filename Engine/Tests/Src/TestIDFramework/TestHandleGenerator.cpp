#include <doctest/doctest.h>

#include "Xuzumi/IDFramework/HandleGenerator.hpp"

TEST_CASE("Xuzumi::Internal::HandleGenerator")
{
	SUBCASE("Basic functionality")
	{
		Xuzumi::Internal::HandleGenerator gen;

		CHECK_FALSE(gen.Exists(0));
		CHECK_FALSE(gen.Exists(Xuzumi::Internal::kInvalidHandleID));

		Xuzumi::Internal::HandleID h1 = gen.Create();
		Xuzumi::Internal::HandleID h2 = gen.Create();
		Xuzumi::Internal::HandleID h3 = gen.Create();

		CHECK(h1 == 0);
		CHECK(h2 == 1);
		CHECK(h3 == 2);
		CHECK(gen.Exists(h1));
		CHECK(gen.Exists(h2));
		CHECK(gen.Exists(h3));

		gen.Destroy(h2);
		gen.Destroy(h3);

		CHECK(gen.Exists(h1));
		CHECK_FALSE(gen.Exists(h2));
		CHECK_FALSE(gen.Exists(h3));

		h2 = gen.Create();
		h3 = gen.Create();

		CHECK(h1 == 0);
		CHECK(h2 == 1);
		CHECK(h3 == 2);
		CHECK(gen.Exists(h1));
		CHECK(gen.Exists(h2));
		CHECK(gen.Exists(h3));
	}

	SUBCASE("Invalid handle")
	{
		Xuzumi::Internal::HandleGenerator gen;

		CHECK_FALSE(gen.Exists(Xuzumi::Internal::kInvalidHandleID));

		Xuzumi::Internal::HandleID h1 = gen.Create();
		CHECK(h1 == 0);
		CHECK(gen.Exists(h1));

		gen.Destroy(Xuzumi::Internal::kInvalidHandleID);
		CHECK(gen.Exists(h1));
	}

	SUBCASE("Reuse handles")
	{
		Xuzumi::Internal::HandleGenerator gen;

		Xuzumi::Internal::HandleID h1 = gen.Create();
		Xuzumi::Internal::HandleID h2 = gen.Create();

		gen.Destroy(h1);
		gen.Destroy(h2);

		h1 = gen.Create();
		h2 = gen.Create();

		CHECK(h1 == 0);
		CHECK(h2 == 1);
		CHECK(gen.Exists(h1));
		CHECK(gen.Exists(h2));
	}
}

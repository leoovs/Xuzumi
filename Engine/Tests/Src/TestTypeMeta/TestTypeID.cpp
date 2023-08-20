#include <doctest/doctest.h>

#include "Xuzumi/TypeMeta/TypeID.hpp"

TEST_CASE("Xuzumi::TypeID")
{
	SUBCASE("Basic functionality")
	{
		struct MockBaseType {};
		struct MockDerivedType {};

		CHECK(
			Xuzumi::GetTypeID<MockBaseType>() != Xuzumi::GetTypeID<MockBaseType&>());	

		CHECK(
			Xuzumi::GetTypeID<MockBaseType>() != Xuzumi::GetTypeID<MockDerivedType>());
	
		struct TypeThatHasNeverBeenMetBefore {};
		Xuzumi::TypeID id1 = Xuzumi::GetTypeID<TypeThatHasNeverBeenMetBefore>();

		struct TypeThatHasNeverBeenMetToo {};
		Xuzumi::TypeID id2 = Xuzumi::GetTypeID<TypeThatHasNeverBeenMetToo>();

		CHECK(id1 + 1 == id2);
	}
}

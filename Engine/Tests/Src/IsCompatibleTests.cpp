#include "doctest/doctest.h"

#include "Xuzumi/Core/Templates/IsCompatible.hpp"

TEST_CASE("Xuzumi::IsCompatible<FirstT, SecondT>")
{
	SUBCASE("Array compatibility")
	{
		CHECK(Xuzumi::IsCompatibleV<int, int>);
		CHECK(Xuzumi::IsCompatibleV<int, int[]>);
		CHECK(Xuzumi::IsCompatibleV<int[], int[]>);
		CHECK_FALSE(Xuzumi::IsCompatibleV<int[], int>);
	}
	
	SUBCASE("Parent and child class compatibility")
	{
		class MockBase {};
		class MockDerived : public MockBase {};

		CHECK_FALSE(Xuzumi::IsCompatibleV<MockBase, MockDerived>);
		CHECK(std::is_convertible_v<MockDerived*, MockBase*>);
		CHECK(Xuzumi::IsCompatibleV<MockDerived, MockBase>);
		CHECK_FALSE(Xuzumi::IsCompatibleV<MockDerived[], MockBase>);
		CHECK_FALSE(Xuzumi::IsCompatibleV<MockBase[], MockDerived>);
	}
}

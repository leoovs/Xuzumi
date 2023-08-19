#include "doctest/doctest.h"

#include "Xuzumi/Memory/SharedPtr.hpp"

TEST_CASE("Xuzumi::SharedPtr")
{
	SUBCASE("RAII")
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

		Xuzumi::SharedPtr<MockResource> externalRef;
		CHECK_FALSE(bool(externalRef));
		CHECK(externalRef.UseCount() == 0);
		
		{
			Xuzumi::SharedPtr<MockResource> res;

			CHECK(0 == sInstanceCounter);
			CHECK_FALSE(bool(res));
			CHECK(res.UseCount() == 0);

			res = Xuzumi::MakeShared<MockResource>();

			CHECK(1 == sInstanceCounter);
			CHECK(bool(res));
			CHECK(res.UseCount() == 1);

			externalRef = res;
			CHECK(1 == sInstanceCounter);
			CHECK(bool(externalRef));
			CHECK(res.UseCount() == externalRef.UseCount());
			CHECK(res.UseCount() == 2);
		}

		CHECK(1 == sInstanceCounter);
		externalRef.Reset();
		CHECK(0 == sInstanceCounter);
	}

	SUBCASE("Type information")
	{
		struct MockBase {};
		struct MockDerived : MockBase {};

		Xuzumi::SharedPtr<void> any;
		any = Xuzumi::MakeShared<MockDerived>();

		CHECK_FALSE(any.Holds<MockBase>());
		CHECK_FALSE(any.Holds<void>());
		CHECK(any.Holds<MockDerived>());

		auto derived = any.As<MockDerived>();

		CHECK(derived.Get());
		CHECK(derived.UseCount() == any.UseCount());
		CHECK(derived.UseCount() == 2);
	
		Xuzumi::SharedPtr<MockBase> base = derived;

		CHECK(base.Get());
		CHECK(base.UseCount() == derived.UseCount());
		CHECK(base.UseCount() == any.UseCount());
		CHECK(base.UseCount() == 3);
		CHECK_FALSE(base.Holds<MockBase>());
		CHECK(base.Holds<MockDerived>());
	}
}

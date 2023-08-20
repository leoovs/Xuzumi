#include <doctest/doctest.h>

#include "Xuzumi/Memory/Referencer.hpp"

TEST_CASE("Xuzumi::Internal::Referencer")
{
	SUBCASE("Ownership")
	{
		static bool sDeleted;

		struct MockResource {};
		struct MockResourceDeleter
		{
			void operator()(MockResource* res)
			{
				sDeleted = true;
				delete res;
			}
		};

		Xuzumi::Internal::Referencer ref;

		CHECK(ref.GetResourceTypeInfo() == Xuzumi::TypeInfo());
		CHECK(ref.UseCount() == 0);

		ref.Bind<
			Xuzumi::Internal::ReferencingControlBlock<
				MockResource,
				MockResourceDeleter
			>
		>(new MockResource(), MockResourceDeleter());

		CHECK(ref.UseCount() == 1);
		CHECK(ref.GetResourceTypeInfo() == Xuzumi::TypeInfo::Get<MockResource>());

		ref.Strongify(); // Nothing should happen.
		CHECK(ref.UseCount() == 1);

		ref.Weakify();	
		CHECK(sDeleted);
		CHECK(ref.UseCount() == 0);
	}
}

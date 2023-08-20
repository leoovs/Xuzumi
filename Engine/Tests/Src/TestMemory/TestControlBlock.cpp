#include <doctest/doctest.h>

#include "Xuzumi/Memory/ControlBlock.hpp"

TEST_CASE("Xuzumi::Internal control blocks")
{
	SUBCASE("Ownership")
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

		struct MockResourceDeleter
		{
			void operator()(MockResource* res)
			{
				delete res;
			}
		};

		auto resource = new MockResource();

		Xuzumi::Internal::IControlBlock* block = nullptr;
		block = new Xuzumi::Internal::ReferencingControlBlock<
			MockResource,
			MockResourceDeleter
		>(resource, MockResourceDeleter());	
	
		CHECK(block->GetStrongRefs() == 1);
		CHECK(block->HasNoWeakRefs());
		CHECK(1 == sInstanceCounter);
		CHECK(block->ResourceIsAlive());

		block->IncrementWeakRefs();

		CHECK(block->GetStrongRefs() == 1);
		CHECK_FALSE(block->HasNoWeakRefs());
		CHECK(block->ResourceIsAlive());

		block->DecrementStrongRefs();

		CHECK(block->GetStrongRefs() == 0);
		CHECK_FALSE(block->HasNoWeakRefs());
		CHECK_FALSE(block->ResourceIsAlive());
		CHECK(0 == sInstanceCounter);

		block->DecrementWeakRefs();
		
		CHECK(block->GetStrongRefs() == 0);
		CHECK(block->HasNoWeakRefs());
		CHECK_FALSE(block->ResourceIsAlive());
	}
}

#include <doctest/doctest.h>

#include "Xuzumi/Memory/FactoryExpirationGuard.hpp"

TEST_CASE("Xuzumi::Internal::ResourceDeleterGuard")
{
	SUBCASE("Basic functionality")
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

		class MockFactory
		{
		public:
			std::shared_ptr<MockResource> CreateResource()
			{
				return std::shared_ptr<MockResource>(
					new MockResource(),
					mDangleGuard.MakeDangleProtectedDeleter<MockResource>(
						&MockFactory::DestroyResource
					)
				);
			}

		private:
			void DestroyResource(MockResource* res)
			{
				delete res;
			}

			Xuzumi::Internal::FactoryExpirationGuard<MockFactory> mDangleGuard = this;
		};

		auto factory = new MockFactory();
		CHECK(0 == sInstanceCounter);

		std::shared_ptr<MockResource> res = factory->CreateResource();
		CHECK(1 == sInstanceCounter);

		res.reset();
		CHECK(0 == sInstanceCounter);

		res = factory->CreateResource();
		delete factory;

		res.reset();
		CHECK(1 == sInstanceCounter);
	}
}

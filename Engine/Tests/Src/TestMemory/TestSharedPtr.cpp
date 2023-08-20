#include <doctest/doctest.h>

#include "Xuzumi/Memory/SharedPtr.hpp"

TEST_CASE("Xuzumi::SharedPtr<T>")
{
	SUBCASE("Ownership sharing and RAII")
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

		constexpr std::size_t kArraySize = 15;

		Xuzumi::SharedPtr<MockResource[]> externalRef;
		CHECK_FALSE(bool(externalRef));
		CHECK(externalRef.UseCount() == 0);
		CHECK(externalRef.GetResourceTypeInfo() == Xuzumi::TypeInfo());
		
		{
			Xuzumi::SharedPtr<MockResource[]> res;

			CHECK(0 == sInstanceCounter);
			CHECK_FALSE(bool(res));
			CHECK(res.UseCount() == 0);
			CHECK(res.GetResourceTypeInfo() == Xuzumi::TypeInfo());

			res = Xuzumi::MakeShared<MockResource[]>(kArraySize);

			CHECK(kArraySize == sInstanceCounter);
			CHECK(bool(res));
			CHECK(res.UseCount() == 1);
			CHECK_FALSE(res.GetResourceTypeInfo()
				== Xuzumi::TypeInfo::Get<MockResource>());
			CHECK(res.GetResourceTypeInfo()
				== Xuzumi::TypeInfo::Get<MockResource[]>());

			externalRef = res;
			CHECK(kArraySize == sInstanceCounter);
			CHECK(bool(externalRef));
			CHECK(res.UseCount() == externalRef.UseCount());
			CHECK(res.UseCount() == 2);
		}

		CHECK(kArraySize == sInstanceCounter);
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

	SUBCASE("Custom deleter")
	{
		static bool sReleased;

		struct MockResource
		{
			static MockResource* Create()
			{
				static MockResource sInstance;
				return &sInstance;
			}

			void Release()
			{
				sReleased = true;	
			}
		};

		Xuzumi::SharedPtr<MockResource> res;
		res.Reset(
			MockResource::Create(),
			[](MockResource* p)
			{
				p->Release();
			}
		);

		res = nullptr;

		CHECK(sReleased);
	}

	SUBCASE("Passing by value as an argument")
	{
		auto takesInPtr = [](Xuzumi::SharedPtr<int> byValue)
		{
			CHECK(5 == *byValue);
			CHECK(byValue.UseCount() == 2);
		};

		auto ptr = Xuzumi::MakeShared<int>(5);

		CHECK(ptr.UseCount() == 1);
		takesInPtr(ptr);
		CHECK(ptr.UseCount() == 1);
	}
}

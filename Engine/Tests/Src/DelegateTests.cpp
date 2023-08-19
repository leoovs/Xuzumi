#include "doctest/doctest.h"

#include "Xuzumi/Core/Delegate.hpp"

TEST_CASE("Xuzumi::Delegate")
{
	SUBCASE("Empty delegate")
	{
		Xuzumi::Delegate<std::string(int)> iToStr;
		CHECK_FALSE(bool(iToStr));
	}

	SUBCASE("Delegate assignment")
	{
		Xuzumi::Delegate<int(float)> mul2i;
		mul2i = [](float) -> int 
		{
			return 0xDEAD;
		};

		CHECK(mul2i(0) == 0xDEAD);	
		CHECK(mul2i(10.5) == 0xDEAD);	

		mul2i = [](float val) -> int
		{
			return static_cast<int>(val * 2);
		};

		CHECK(mul2i(5.0f) == 10);
		CHECK(mul2i(5.5f) == 11);	
		CHECK(mul2i(0.1f) == 0);	
	}

	SUBCASE("Delegate argument passing")
	{
		struct Mock {} inst;
	
		auto observer = [&inst](Mock& o)
		{
			CHECK(&o == &inst);
		};

		Xuzumi::Delegate<void(Mock&)> delegate = observer;
		delegate(inst);
	}

	SUBCASE("Delegate argument copy")
	{
		static int sCopyCounter = 0;
		static int sMoveCounter = 0;

		struct CopyableMovableMock
		{
			CopyableMovableMock() = default;

			CopyableMovableMock(CopyableMovableMock&& other)
			{
				sMoveCounter++;
			}	

			CopyableMovableMock(const CopyableMovableMock&)
			{
				sCopyCounter++;
			}	
		} inst;

		auto observer = [](CopyableMovableMock byValue) {};

		Xuzumi::Delegate<void(CopyableMovableMock)> delegate(observer);
		delegate(inst);

		CHECK(1 == sCopyCounter);
		CHECK(1 == sMoveCounter);
	}
}

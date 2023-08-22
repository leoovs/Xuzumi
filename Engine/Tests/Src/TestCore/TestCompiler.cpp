#include <doctest/doctest.h>

#include "Xuzumi/Core/Compiler.hpp"

TEST_CASE("Xuzumi::GetCompilerName")
{
	SUBCASE("Basic functionality")
	{
#if DOCTEST_GCC
		CHECK("GCC" == Xuzumi::GetCompilerName());
		CHECK(XZ_CURRENT_COMPILER_IS(GCC));
#elif DOCTEST_CLANG
		CHECK("Clang" == Xuzumi::GetCompilerName());
		CHECK(XZ_CURRENT_COMPILER_IS(CLANG));
#elif DOCTEST_MSVC
		CHECK("MSVC" == Xuzumi::GetCompilerName());
		CHECK(XZ_CURRENT_COMPILER_IS(MSVC));
#else
#	error "Xuzumi: unknown compiler"
#endif
	}
}

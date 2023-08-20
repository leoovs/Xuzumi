#include <doctest/doctest.h>

#include "Xuzumi/Core/StringUtils.hpp"

TEST_CASE("Xuzumi::FormatString")
{
	CHECK(Xuzumi::FormatString("%d + %s", 5, "123ABC")
		== std::string("5 + 123ABC"));

	CHECK(Xuzumi::FormatString("", 5, 160.0f, "a", 'c').empty());
}

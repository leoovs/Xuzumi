#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	std::string FormatString(std::string_view format, ...);
	std::string FormatString(std::string_view format, va_list args);
}

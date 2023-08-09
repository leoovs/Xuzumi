#ifndef XUZUMI_CORE_STRINGUTILS_HPP_
#define XUZUMI_CORE_STRINGUTILS_HPP_

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	std::string FormatString(std::string_view format, ...);
	std::string FormatStringV(std::string_view format, va_list args);
}

#endif // XUZUMI_CORE_STRINGUTILS_HPP_

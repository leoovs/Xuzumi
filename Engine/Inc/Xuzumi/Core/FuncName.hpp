#pragma once

#include "Xuzumi/Core/Compiler.hpp"

#if XZ_CURRENT_COMPILER_IS(GCC)
#	define XZ_FUNCNAME __PRETTY_FUNCTION__
#elif XZ_CURRENT_COMPILER_IS(CLANG)
#	define XZ_FUNCNAME __PRETTY_FUNCTION__
#elif XZ_CURRENT_COMPILER_IS(MSVC)
#	define XZ_FUNCNAME __FUNCSIG__
#else
#	define XZ_FUNCNAME __func__
#endif

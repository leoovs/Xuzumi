/**
 * @file Xuzumi/Core/FuncName.hpp
 * 
 * @brief Defines a macro to retrieve function name inside of its scope.
 * 
 * Provides XZ_FUNCNAME macro based on the compiler in use, ensuring
 * compatibility across different compilers. 
 */

#pragma once

#include "Xuzumi/Core/CompilerInfo.hpp"

#if XZ_CURRENT_COMPILER_IS(GCC)
#	define XZ_FUNCNAME __PRETTY_FUNCTION__
#elif XZ_CURRENT_COMPILER_IS(CLANG)
#	define XZ_FUNCNAME __PRETTY_FUNCTION__
#elif XZ_CURRENT_COMPILER_IS(MSVC)
#	define XZ_FUNCNAME __FUNCSIG__
#else
#	define XZ_FUNCNAME __func__
#endif

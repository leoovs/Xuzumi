/**
 * @file Xuzumi/Core/FuncName.hpp
 * 
 * @brief Defines utility macro for querying
 * current function name.
 */

#ifndef XUZUMI_CORE_FUNCNAME_HPP_
#define XUZUMI_CORE_FUNCNAME_HPP_

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

#endif // XUZUMI_CORE_FUNCNAME_HPP_

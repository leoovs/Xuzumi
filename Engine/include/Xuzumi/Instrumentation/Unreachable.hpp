#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Core/CompilerInfo.hpp"

#if XZ_CURRENT_COMPILER_IS(GCC)
#	define XZ_MARK_UNREACHABLE() __builtin_unreachable()
#elif XZ_CURRENT_COMPILER_IS(CLANG)
#	define XZ_MARK_UNREACHABLE() __builtin_unreachable()
#elif XZ_CURRENT_COMPILER_IS(MSVC)
#	define XZ_MARK_UNREACHABLE() __assume(0)
#else
#	error "Xuzumi: unknown compiler"
#endif

#ifndef NDEBUG
#	undef XZ_MARK_UNREACHABLE
#	define XZ_MARK_UNREACHABLE() assert(false && "Attempt to execute an unreachable code")
#endif

#pragma once

#define XZ_COMPILER_ID_GCC   0u
#define XZ_COMPILER_ID_CLANG 1u
#define XZ_COMPILER_ID_MSVC  2u

#if defined(__GNUG__) && !defined(__clang__)
#	define XZ_COMPILER_ID XZ_COMILER_ID_GCC
#elif defined(__clang__)
#	define XZ_COMPILER_ID XZ_COMPILER_ID_CLANG
#elif defined(_MSC_VER)
#	define XZ_COMPILER_ID XZ_COMPILER_ID_MSVC
#else
#	error "Xuzumi: unknown compiler."
#endif

#define XZ_CURRENT_COMPILER_IS(compiler) XZ_COMPILER_ID == XZ_COMPILER_ID_##compiler

namespace Xuzumi
{
	// TODO: implement GetCompilerName function.
}

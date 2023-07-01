/**
 * @file Xuzumi/Core/Compiler.hpp
 * 
 * @brief Compiler identification macros for Xuzumi engine.
 */

#ifndef XUZUMI_CORE_COMPILER_HPP_
#define XUZUMI_CORE_COMPILER_HPP_

/**
 * @def XZ_COMPILER_ID_GCC
 * 
 * @brief Compiler ID for GCC.
 */
#define XZ_COMPILER_ID_GCC 0u

/**
 * @def XZ_COMPILER_ID_CLANG
 * 
 * @brief Compiler ID for CLANG.
 */
#define XZ_COMPILER_ID_CLANG 1u

/**
 * @def XZ_COMPILER_ID_MSVC
 * 
 * @brief Compiler ID for MSVC.
 */
#define XZ_COMPILER_ID_MSVC 2u

#if defined(__GNUG__) && !defined(__clang__)
#	define XZ_COMPILER_ID XZ_COMILER_ID_GCC
#elif defined(__clang__)
#	define XZ_COMPILER_ID XZ_COMPILER_ID_CLANG
#elif defined(_MSC_VER)
#	define XZ_COMPILER_ID XZ_COMPILER_ID_MSVC
#else
#	error "Xuzumi: unknown compiler."
#endif

/**
 * @def XZ_CURRENT_COMPILER_IS(compiler)
 * 
 * @brief Macro to check if the current compiler matches the specified compiler.
 * 
 * @param compiler The desired compiler to check against (GCC, CLANG, MSVC).
 */
#define XZ_CURRENT_COMPILER_IS(compiler) XZ_COMPILER_ID == XZ_COMPILER_ID_##compiler

namespace Xuzumi
{
	// TODO: implement GetCompilerName function.
}

#endif // XUZUMI_CORE_COMPILER_HPP_

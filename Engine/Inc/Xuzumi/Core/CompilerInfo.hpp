/**
 * @file Xuzumi/Core/CompilerInfo.hpp
 * 
 * @brief Provides utilities to detect information about the compiler used to
 * build Xuzumi.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"

/**
 * @def XZ_COMPILER_ID_GCC
 * 
 * @brief Identifier for the GNU Compiler.
 */
#define XZ_COMPILER_ID_GCC 0u

/**
 * @def XZ_COMPILER_ID_CLANG
 * 
 * @brief Identifier for the Clang compiler. 
 */
#define XZ_COMPILER_ID_CLANG 1u

/**
 * @def XZ_COMPILER_ID_MSVC
 * 
 * @brief Identifier for the Visual C++ compiler. 
 */
#define XZ_COMPILER_ID_MSVC 2u

/**
 * @brief Identifies the compiler used to build Xuzumi.
 * 
 * XZ_COMPILER_ID is set to one of the predefined compiler identifiers based on
 * detected compiler. If the compiler is not recognized, an error is generated.
 */
#if defined(__GNUG__) && !defined(__clang__)
#	define XZ_COMPILER_ID XZ_COMPILER_ID_GCC
#elif defined(__clang__)
#	define XZ_COMPILER_ID XZ_COMPILER_ID_CLANG
#elif defined(_MSC_VER)
#	define XZ_COMPILER_ID XZ_COMPILER_ID_MSVC
#else
#	error "Xuzumi: unknown compiler"
#endif

/**
 * @def XZ_CURRENT_COMPILER_IS(compiler)
 * 
 * @brief Checks whether the specified @a compiler is used to compile Xuzumi.
 * 
 * @param compiler Name of the compiler, must be one of the following:
 * - GCC
 * - CLANG
 * - MSVC
 */
#define XZ_CURRENT_COMPILER_IS(compiler) XZ_COMPILER_ID == XZ_COMPILER_ID_##compiler

namespace Xuzumi
{
	/**
	 * @brief Retrieves the name of the compiler used to build Xuzumi.
	 * 
	 * @return An `std::string_view` instance containing the name of the compiler.
	 */
	constexpr std::string_view GetCompilerName()
	{
		constexpr std::string_view kCompilerNames[]
		{
			"GCC",
			"Clang",
			"MSVC"
		};

		return kCompilerNames[XZ_COMPILER_ID];
	}
}

/**
 * @file Xuzumi/Core/StringUtils.hpp
 * 
 * @brief This file provides utility functions for formatting strings. 
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @brief Formats a string using C-style format specifiers.
	 * 
	 * This function allows you to format a string similar to the `printf` family
	 * of functions.
	 * 
	 * @param format The format string.
	 * @param ... Additional arguments to be formatted in the string.
	 * @return New `std::string` instance containing the formatted result. 
	 */
	std::string FormatString(std::string_view format, ...);

	/**
	 * @brief Formats a string using C-style format specifiers and a `va_list` of
	 * arguments.
	 * 
	 * @param format The format string.
	 * @param args A `va_list` arguments to be formatted in the string.
	 * @return New `std::string` instance containing the formatted result.
	 */
	std::string FormatString(std::string_view format, va_list args);
}

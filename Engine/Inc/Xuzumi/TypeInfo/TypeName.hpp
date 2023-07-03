/**
 * @file Xuzumi/TypeInfo/TypeName.hpp
 * 
 * @brief Defines functions to query type name as a string.
 */

#ifndef XUZUMI_TYPEINFO_TYPENAME_HPP_
#define XUZUMI_TYPEINFO_TYPENAME_HPP_

#include <Xuzumi/Precompiled.hpp>
#include <Xuzumi/Core/Compiler.hpp>
#include <Xuzumi/Core/FuncName.hpp>

namespace Xuzumi::Internal
{
#if XZ_CURRENT_COMPILER_IS(GCC)
	template<typename ClassT>
	constexpr std::string_view GetStaticTypeName()
	{
		constexpr std::string_view funcName = XZ_FUNCNAME;
		constexpr std::size_t typeNameStart = funcName.find_first_of("=") + 2;
		constexpr std::size_t typeNameEnd = funcName.find_first_of(";");

		return funcName.substr(typeNameStart, typeNameEnd - typeNameStart);
	}
#elif XZ_CURRENT_COMPILER_IS(CLANG)
	template<typename ClassT>
	constexpr std::string_view GetStaticTypeName()
	{
		constexpr std::string_view funcName = XZ_FUNCNAME;
		constexpr std::size_t typeNameStart = funcName.find_first_of("=") + 2;
		constexpr std::size_t typeNameEnd = funcName.find_first_of("]");

		return funcName.substr(typeNameStart, typeNameEnd - typeNameStart);
	}
#elif XZ_CURRENT_COMPILER_IS(MSVC)
	template<typename ClassT>
	constexpr std::string_view GetStaticTypeName()
	{
		constexpr std::string_view funcSig = XZ_FUNCNAME;
		constexpr std::string_view funcName = __func__;	
	
		constexpr std::string_view funcNameStart = funcSig.substr(
			funcSig.find(funcName)
		);

		constexpr std::size_t typeNameStart = funcNameStart.find_first_of("<") + 1;
		constexpr std::size_t typeNameEnd = funcNameStart.find_last_of(">");

		return funcNameStart.substr(typeNameStart, typeNameEnd - typeNameStart);
	}
#else
#	error "Unknown compiler."
#endif
}

namespace Xuzumi
{
	/**
	 * @brief Retrieves type name and converts it to std::string.
	 * 
	 * @tparam ClassT A class to query its type name.
	 */
	template<typename ClassT>
	std::string GetTypeName()
	{
		return std::string(Internal::GetStaticTypeName<ClassT>());	
	}

	/**
	 * @brief Convenience function that retrieves type name of an object.
	 * 
	 * @tparam ClassT A class to parse its type name. Supposed to be implicit.
	 * 
	 * @param classInstance A class instance to take its type name.
	 */
	template<typename ClassT>
	std::string GetTypeName(const ClassT& classInstance)
	{
		return GetTypeName<ClassT>();
	}
}

#endif // XUZUMI_TYPEINFO_TYPENAME_HPP_

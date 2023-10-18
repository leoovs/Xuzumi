#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Core/CompilerInfo.hpp"
#include "Xuzumi/Core/FuncName.hpp"

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
#	error "Xuzumi: unknown compiler"
#endif
}

namespace Xuzumi
{
	/**
	 * @brief Retrieves a human-readable string containing the name of the
	 * @p ClassT type. 
	 * 
	 * @tparam ClassT The type which name is to be retrieved.
	 *
	 * @return An `std::string` instance containing the name of the type.
	 */
	template<typename ClassT>
	std::string GetTypeName()
	{
		return std::string(Internal::GetStaticTypeName<ClassT>());	
	}

	/**
	 * @brief Retrieves a human-readable string containing the name of the
	 * @p classInstance type. 
	 * 
	 * @tparam ClassT The type of the @p classInstance.
	 * 
	 * @param classInstance A reference to an object of the @p ClassT type.
	 * 
	 * @return An `std::string` instance containing the name of the type.
	 */
	template<typename ClassT>
	std::string GetTypeName(const ClassT& classInstance)
	{
		return GetTypeName<ClassT>();
	}
}

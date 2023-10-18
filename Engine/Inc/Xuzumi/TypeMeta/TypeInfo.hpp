#pragma once

#include "Xuzumi/TypeMeta/TypeID.hpp"
#include "Xuzumi/TypeMeta/TypeName.hpp"

namespace Xuzumi
{
	// TODO: maybe add SizeOf and AlignOf fields.

	/**
	 * @brief Type used to capture useful type characteristics including the type
	 * identifier.
	 * 
	 * Extensively used for type introspection.
	 */
	struct TypeInfo
	{
		/**
		 * @brief A type identifier.
		 */
		TypeID ID = kInvalidTypeID;

		/**
		 * @brief A type name.
		 */
		std::string Name;	

		/**
		 * @brief Retrieves `TypeInfo` instance for @p T.
		 *
		 * @tparam T The type which type information is to be retrieved.
		 * 
		 * @return A `TypeInfo` instance containing @p T type information.
		 */
		template<typename T>
		static TypeInfo Get()
		{
			return
			{
				GetTypeID<T>(),
				GetTypeName<T>()
			};
		}

		/**
		 * @brief Converts `*this` into human-readable format.
		 *
		 * @return An `std::string` instance containing a string representation of
		 * `*this`.
		 */
		std::string ToString() const;

		/**
		 * @brief Checks whether `*this` references a valid type.
		 *
		 * @retval true `*this` contains valid type identifier.
		 * @retval false `*this` contains invalid type identifier.
		 */
		explicit operator bool() const;

		/**
		 * @brief Checks whether `*this` and @p other refer to the same type.
		 *
		 * @retval true @p other refers to the same type as `*this`.
		 * @retval false @p other and `*this` refer to different types.
		 */
		bool operator==(const TypeInfo& other) const;
		
		/**
		 * @brief Checks whether `*this` and @p other refer to distinct types.
		 *
		 * @retval true @p other and `*this` refer to different types.
		 * @retval false @p other refers to the same type as `*this`.
		 */
		bool operator!=(const TypeInfo& other) const;
	};	
}
